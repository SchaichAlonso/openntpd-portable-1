#include <cstddef>
#include <cstdint>

#include <chrono>
#include <iostream>
#include <thread>

#include <dlfcn.h>

static int (*gRealMain)(int, char **, char **) = nullptr;

// Our fuzz target
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size);
// Provided by libFuzzer
extern "C" int LLVMFuzzerRunDriver(int *argc, char ***argv,
                  int (*UserCb)(const uint8_t *Data, size_t Size));


int our_main(int argc, char** argv, char** envp) {

    if(getenv("FUZZER_RUNNING")) {

        return gRealMain(argc, argv, envp);

    } else {
        putenv("FUZZER_RUNNING=1");

        std::thread real_main([&]() {
            gRealMain(argc, argv, envp);
        });

        // uncomment during debugging
        //std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        std::cout << "[premain] Starting fuzzer..." << std::endl;
        return LLVMFuzzerRunDriver(&argc, &argv, LLVMFuzzerTestOneInput);
    }
    
}

__attribute__((visibility("default"))) __attribute__((unused)) extern "C" int
__libc_start_main(int (*main)(int, char **, char **), int argc, char **argv,
                  int (*init)(int, char **, char **), void (*fini)(void),
                  void (*rtld_fini)(void), void *stack_end) {
  gRealMain = main;
  decltype(&__libc_start_main) real_start_main =
      reinterpret_cast<decltype(&__libc_start_main)>(
          dlsym(RTLD_NEXT, "__libc_start_main"));
  return real_start_main(our_main, argc, argv, init,
                         fini, rtld_fini, stack_end);
}