#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "xml.h"
#include "stdio_config.h"
#include "getopt.h"
#include "strlist.h"
#include "log.h"
#include <string.h>

void show_help() {
	printf_s(
"sportreader [options] <Location>\n\
Options:\n\
-v  --verbose           Set loglevel to verbose level.\n\
    --loglevel <level>  Set loglevel.\n\
                        Available values: verbose, warning, error, quiet\n\
-q  --quiet             Set loglevel to quiet level.\n\
                        In quiet level, only display output infomation.\n\
-V  --version           Show version and then exit.\n\
-h  --help              Show help and then exit.\n");
}

int main(int argc, char* argv[]) {
	const char *optstr = "vqVh";
	struct option opts[] = {
		{"verbose", 0, NULL, 'v'},
		{"quiet", 0, NULL, 'q'},
		{"version", 0, NULL, 'V'},
		{"help", 0, NULL, 'h'},
		{"loglevel", 1, NULL, 129}
	};
	int c;
	LIBSR_LOG_LEVEL log_level = LIBSR_WARNING_LEVEL;
	StrList *locs = new_strList();
	if (!locs) return 1;
	while ((c = getopt_long(argc, argv, optstr, opts, NULL)) != -1) {
		switch (c) {
		case 'v':
			log_level = LIBSR_VERBOSE_LEVEL;
			break;
		case 'V':
			printf_s("Version: %s.\n", VERSION);
			printf_s("Build Time: %s %s.\n", __DATE__, __TIME__);
#ifdef _MSC_VER
			printf_s("Complier: MSVC v%02d.%02d.%05d.%02d\n", _MSC_FULL_VER / 10000000, (_MSC_FULL_VER % 10000000) / 100000, _MSC_FULL_VER % 100000, _MSC_BUILD);
#endif
#ifdef __GNUC__
			printf_s("Compiler: GCC v%s\n", __VERSION__);
#endif
#if __clang__
			printf_s("Compiler: Clang v%s\n", __clang_version__);
#endif
			free_strList(locs);
			return 0;
		case 'h':
			show_help();
			free_strList(locs);
			return 0;
		case 'q':
			log_level = LIBSR_QUIET_LEVEL;
			break;
		case 129:
			if (!strcmp(optarg, "verbose")) log_level = LIBSR_VERBOSE_LEVEL;
			else if (!strcmp(optarg, "warning")) log_level = LIBSR_WARNING_LEVEL;
			else if (!strcmp(optarg, "error")) log_level = LIBSR_ERROR_LEVEL;
			else if (!strcmp(optarg, "quiet")) log_level = LIBSR_QUIET_LEVEL;
			else {
				printf_s("Unknown value \"%s\" for option loglevel.\n", optarg);
			}
			break;
		case '?':
			printf_s("Use -h to show all available options.\n");
			free_strList(locs);
			return -1;
		default:
			break;
		}
	}
	int t;
	for (t = optind; t < argc; t++) {
		locs->append(locs, argv[t]);
	}
	set_libsr_log_level(log_level);
	if (locs->first) {
		StrNode *n = NULL;
		for (n = locs->first; n; n = n->next) {
			if (n->cur) {
				libsr_log_verbose("Try to parse file: %s.\n", n->cur);
				XMLParseResult result = readXML(n->cur);
				if (result.status != PARSE_OK) {
					libsr_log_warning("Can not parse XML file: %s.\n", n->cur);
					continue;
				}
				if (result.tcd != NULL) free_tcd(result.tcd);
			}
		}
	}
	free_strList(locs);
    return 0;
}
