#
# Copyright (c) 2014-2015 Brent Cook
# Parts based on Makefile.in from Portable OpenNTPD:
# Copyright (c) 2004-2008 Darren Tucker.
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

SUBDIRS = include compat src
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = README.md INSTALL VERSION ntpd.conf

install-exec-hook:
	@if [ ! -f "$(DESTDIR)$(sysconfdir)/ntpd.conf" ]; then \
		$(INSTALL) -m 644 "$(srcdir)/ntpd.conf" "$(DESTDIR)$(sysconfdir)/ntpd.conf"; \
	else \
		echo; \
		echo " $(DESTDIR)$(sysconfdir)/ntpd.conf already exists, install will not overwrite"; \
	fi

uninstall-local:
	@if cmp -s "$(DESTDIR)$(sysconfdir)/ntpd.conf" "$(srcdir)/ntpd.conf"; then \
		rm -f "$(DESTDIR)$(sysconfdir)/ntpd.conf"; \
	fi
	@rm -f "$(DESTDIR)$(sbindir)/ntpctl"

main_hook:
	@echo "Building $@"
	${CXX} ${CXXFLAGS} -g -c libfuzzer_no_main/main_hook.cpp -o libfuzzer_no_main/main_hook.o 

# in case we want to rebuild target ntpd.o
ntpd:
	@echo "Building $@"
	clang -DSPT_TYPE=SPT_REUSEARGV -DHAVE_ARC4RANDOM=1 -DHAVE_ARC4RANDOM_BUF=1 -DHAVE_ARC4RANDOM_UNIFORM=1 -DHAVE_EXPLICIT_BZERO=1 -DHAVE_GETAUXVAL=1 -DHAVE_ASPRINTF=1 -DHAVE_CLOSEFROM=1 -DHAVE_DAEMON=1 -DHAVE_MEMMEM=1 -DHAVE_POLL=1 -DHAVE_REALLOCARRAY=1 -DHAVE_SETGROUPS=1 -DHAVE_SETREGID=1 -DHAVE_SETRESGID=1 -DHAVE_SETREUID=1 -DHAVE_SETRESUID=1 -DHAVE_SYSCONF=1 -DHAVE_NTP_ADJTIME=1 -DHAVE_ADJTIMEX=1 -DHAVE_CLOCK_GETTIME=1 -DHAVE_CLOCK_GETRES=1 -DHAVE___PROGNAME=1 -DHAVE_VA_COPY=1 -DHAVE___VA_COPY=1 -DHAVE_ERR_H=1 -DHAVE_IFADDRS_H=1 -DHAVE_PATHS_H=1 -DNTPD_USER=\"_ntp\" -DCONSTRAINT_CA=\"/etc/ssl/cert.pem\" -Isrc  -Iinclude -DYYSTYPE_IS_DECLARED -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_SOURCE -D_GNU_SOURCE -fPIC -fsanitize=fuzzer-no-link -fsanitize=address -c src/ntpd.c -o src/ntpd-ntpd.o

ntpd_coverage:
	clang -DSPT_TYPE=SPT_REUSEARGV -DHAVE_ARC4RANDOM=1 -DHAVE_ARC4RANDOM_BUF=1 -DHAVE_ARC4RANDOM_UNIFORM=1 -DHAVE_EXPLICIT_BZERO=1 -DHAVE_GETAUXVAL=1 -DHAVE_ASPRINTF=1 -DHAVE_CLOSEFROM=1 -DHAVE_DAEMON=1 -DHAVE_MEMMEM=1 -DHAVE_POLL=1 -DHAVE_REALLOCARRAY=1 -DHAVE_SETGROUPS=1 -DHAVE_SETREGID=1 -DHAVE_SETRESGID=1 -DHAVE_SETREUID=1 -DHAVE_SETRESUID=1 -DHAVE_SYSCONF=1 -DHAVE_NTP_ADJTIME=1 -DHAVE_ADJTIMEX=1 -DHAVE_CLOCK_GETTIME=1 -DHAVE_CLOCK_GETRES=1 -DHAVE___PROGNAME=1 -DHAVE_VA_COPY=1 -DHAVE___VA_COPY=1 -DHAVE_ERR_H=1 -DHAVE_IFADDRS_H=1 -DHAVE_PATHS_H=1 -DNTPD_USER=\"_ntp\" -DCONSTRAINT_CA=\"/etc/ssl/cert.pem\" -Isrc  -Iinclude -DYYSTYPE_IS_DECLARED -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_SOURCE -D_GNU_SOURCE -fPIC -fsanitize=fuzzer-no-link -fsanitize=address -fprofile-instr-generate -fcoverage-mapping -c src/ntpd.c -o src/ntpd-ntpd-coverage.o

fuzz_harness_external: 
	@echo "Building $@"
	${CXX} ${CXXFLAGS} ${FUZZ_TEST_CXXFLAGS} ${FUZZ_TEST_LDFLAGS} -g -c $@.cpp -o $@.o

ntpd_fuzzer_external:
	@echo "Link $@"
	${CXX} -fsanitize=address -g -o $@ -Wl fuzz_harness_external.o libfuzzer_no_main/main_hook.o -Wl src/ntpd-client.o src/ntpd-config.o -Wl src/ntpd-constraint-disabled.o -Wl src/ntpd-control.o -Wl src/ntpd-log.o -Wl src/ntpd-ntp.o -Wl src/ntpd-ntp_dns.o -Wl src/ntpd-ntp_msg.o -Wl src/ntpd-ntpd.o -Wl src/ntpd-parse.o -Wl src/ntpd-fake-sensors.o -Wl src/ntpd-server.o -Wl src/ntpd-util.o -Wl libfuzzer_no_main/libclang_rt.fuzzer_no_main-x86_64.a -lresolv -lm compat/.libs/libcompat.a -lstdc++

ntpd_fuzzer_coverage_external:
	@echo "Link $@"
	${CXX} -fsanitize=address -fprofile-instr-generate -fcoverage-mapping -g -o $@ -Wl fuzz_harness_external.o libfuzzer_no_main/main_hook.o -Wl src/ntpd-client.o src/ntpd-config.o -Wl src/ntpd-constraint-disabled.o -Wl src/ntpd-control.o -Wl src/ntpd-log.o -Wl src/ntpd-ntp.o -Wl src/ntpd-ntp_dns.o -Wl src/ntpd-ntp_msg.o -Wl src/ntpd-ntpd-coverage.o -Wl src/ntpd-parse.o -Wl src/ntpd-fake-sensors.o -Wl src/ntpd-server.o -Wl src/ntpd-util.o -Wl libfuzzer_no_main/libclang_rt.fuzzer_no_main-x86_64.a -lresolv -lm compat/.libs/libcompat.a -lstdc++

fuzzer_external: fuzz_harness_external ntpd_fuzzer_external ntpd_fuzzer_coverage_external

fuzz_harness_internal: 
	@echo "Building $@"
	${CXX} ${CXXFLAGS} ${FUZZ_TEST_CXXFLAGS} ${FUZZ_TEST_LDFLAGS} -g -c $@.cpp -o $@.o

ntpd_fuzzer_internal:
	@echo "Link $@"
	${CXX} -fsanitize=address -g -o $@ -Wl fuzz_harness_internal.o libfuzzer_no_main/main_hook.o -Wl src/ntpd-client.o src/ntpd-config.o -Wl src/ntpd-constraint-disabled.o -Wl src/ntpd-control.o -Wl src/ntpd-log.o -Wl src/ntpd-ntp.o -Wl src/ntpd-ntp_dns.o -Wl src/ntpd-ntp_msg.o -Wl src/ntpd-ntpd.o -Wl src/ntpd-parse.o -Wl src/ntpd-fake-sensors.o -Wl src/ntpd-server.o -Wl src/ntpd-util.o -Wl libfuzzer_no_main/libclang_rt.fuzzer_no_main-x86_64.a -lresolv -lm compat/.libs/libcompat.a -lstdc++

ntpd_fuzzer_coverage_internal:
	@echo "Link $@"
	${CXX} -fsanitize=address -fprofile-instr-generate -fcoverage-mapping -g -o $@ -Wl fuzz_harness_internal.o libfuzzer_no_main/main_hook.o -Wl src/ntpd-client.o src/ntpd-config.o -Wl src/ntpd-constraint-disabled.o -Wl src/ntpd-control.o -Wl src/ntpd-log.o -Wl src/ntpd-ntp.o -Wl src/ntpd-ntp_dns.o -Wl src/ntpd-ntp_msg.o -Wl src/ntpd-ntpd-coverage.o -Wl src/ntpd-parse.o -Wl src/ntpd-fake-sensors.o -Wl src/ntpd-server.o -Wl src/ntpd-util.o -Wl libfuzzer_no_main/libclang_rt.fuzzer_no_main-x86_64.a -lresolv -lm compat/.libs/libcompat.a -lstdc++

fuzzer_internal: fuzz_harness_internal ntpd_fuzzer_internal ntpd_fuzzer_coverage_internal