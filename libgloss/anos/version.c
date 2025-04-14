/*
 * libanos version string
 * anos - An Operating System
 *
 * Copyright (c) 2025 Ross Bamford
 */

#ifndef VERSTR
#warning Version String not defined (-DVERSTR); Using default
#define VERSTR #unknown
#endif

#define XSTRVER(verstr) #verstr
#define STRVER(xstrver) XSTRVER(xstrver)
#define VERSION STRVER(VERSTR)

const char *libanos_version() { return VERSION; }