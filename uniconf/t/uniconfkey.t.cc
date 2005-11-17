#include "wvtest.h"
#include "uniconfkey.h"

WVTEST_MAIN("slash collapsing")
{
    WVPASSEQ(UniConfKey().printable(), "");
    WVPASSEQ(UniConfKey().numsegments(), 0);
    WVPASSEQ(UniConfKey("").printable(), "");
    WVPASSEQ(UniConfKey("").numsegments(), 0);
    WVPASSEQ(UniConfKey("/").printable(), "");
    WVPASSEQ(UniConfKey("////").printable(), "");
    WVPASSEQ(UniConfKey("///").numsegments(), 0);

    WVPASSEQ(UniConfKey("foo").printable(), "foo");
    WVPASSEQ(UniConfKey("foo").numsegments(), 1);
    WVPASSEQ(UniConfKey("/foo").printable(), "foo");
    WVPASSEQ(UniConfKey("/foo").numsegments(), 1);
    WVPASSEQ(UniConfKey("foo/").printable(), "foo/");
    WVPASSEQ(UniConfKey("foo/").numsegments(), 2);
    WVPASSEQ(UniConfKey("/foo/").printable(), "foo/");
    WVPASSEQ(UniConfKey("/foo/").numsegments(), 2);

    WVPASSEQ(UniConfKey("//bar").printable(), "bar");
    WVPASSEQ(UniConfKey("///bar").printable(), "bar");
    WVPASSEQ(UniConfKey("bar//").printable(), "bar/");
    WVPASSEQ(UniConfKey("bar///").printable(), "bar/");
    WVPASSEQ(UniConfKey("///bar////").printable(), "bar/");

    WVPASSEQ(UniConfKey("fred/barney").printable(), "fred/barney");
    WVPASSEQ(UniConfKey("/fred/barney").printable(), "fred/barney");
    WVPASSEQ(UniConfKey("fred//barney").printable(), "fred/barney");
    WVPASSEQ(UniConfKey("fred///barney").printable(), "fred/barney");
    WVPASSEQ(UniConfKey("/fred///barney").printable(), "fred/barney");
    WVPASSEQ(UniConfKey("///fred///barney").printable(), "fred/barney");
    WVPASSEQ(UniConfKey("fred/barney/").printable(), "fred/barney/");
    WVPASSEQ(UniConfKey("fred/barney///").printable(), "fred/barney/");
    WVPASSEQ(UniConfKey("/fred/barney/").printable(), "fred/barney/");
    WVPASSEQ(UniConfKey("//fred///barney/").printable(), "fred/barney/");
    WVPASSEQ(UniConfKey("///fred////barney///").printable(), "fred/barney/");

    WVPASSEQ(UniConfKey("larry//////curly//////moe").printable(),
	     "larry/curly/moe");
    WVPASSEQ(UniConfKey("larry//////curly//////moe////////").printable(),
	     "larry/curly/moe/");
    WVPASSEQ(UniConfKey("////larry/////curly////moe///////").printable(),
	     "larry/curly/moe/");
}

WVTEST_MAIN("equality")
{
    WVPASS(UniConfKey() == UniConfKey("/"));
    WVPASS(UniConfKey("") == UniConfKey("/"));
    WVPASS(UniConfKey("baz") == UniConfKey("/baz"));
    WVPASS(UniConfKey("ack/nak") == UniConfKey("//ack///nak"));
    WVFAIL(UniConfKey("a") == UniConfKey("a/"));
    WVFAIL(UniConfKey("/a") == UniConfKey("a/"));
}

WVTEST_MAIN("composition")
{
    WVPASS(UniConfKey(UniConfKey("simon"), UniConfKey(""))
	   == UniConfKey("simon/"));
    WVPASSEQ(UniConfKey(UniConfKey("simon"), UniConfKey("")).printable(),
	     "simon/");

    WVPASS(UniConfKey(UniConfKey("simon"), UniConfKey("/"))
	   == UniConfKey("simon/"));
    WVPASSEQ(UniConfKey(UniConfKey("simon"), UniConfKey("/")).printable(),
	     "simon/");

    UniConfKey tmp(UniConfKey("simon"), UniConfKey("/"));
    WVPASS(UniConfKey(tmp, UniConfKey("law")) == UniConfKey("simon/law"));
    WVPASSEQ(UniConfKey(tmp, UniConfKey("law")).printable(), "simon/law");

    WVPASS(UniConfKey(UniConfKey("simon/"), UniConfKey(""))
	   == UniConfKey("simon/"));
    WVPASSEQ(UniConfKey(UniConfKey("simon/"), UniConfKey("")).printable(),
	     "simon/");

    WVPASS(UniConfKey(UniConfKey("simon/"), UniConfKey("law"))
	   == UniConfKey("simon/law"));
    WVPASSEQ(UniConfKey(UniConfKey("simon/"), UniConfKey("law")).printable(),
	     "simon/law");
}

WVTEST_MAIN("subkeys")
{
    WVPASS(UniConfKey().suborsame(UniConfKey("")));
    WVPASS(UniConfKey().suborsame(UniConfKey("cfg/ini")));
    WVPASS(UniConfKey("").suborsame(UniConfKey("cfg/ini")));
    WVPASS(UniConfKey("/").suborsame(UniConfKey("cfg/ini")));
    WVPASS(UniConfKey("cfg").suborsame(UniConfKey("cfg/ini")));
    WVPASS(UniConfKey("cfg/").suborsame(UniConfKey("cfg/ini")));
    WVPASS(UniConfKey("/cfg/ini").suborsame(UniConfKey("cfg/ini")));
    WVFAIL(UniConfKey("/cfg/ini/foo").suborsame(UniConfKey("cfg/ini")));
    WVFAIL(UniConfKey("/ini/cfg").suborsame(UniConfKey("cfg/ini")));

    WVPASSEQ(UniConfKey().subkey(UniConfKey("")).cstr(), "");
    WVPASSEQ(UniConfKey().subkey(UniConfKey("cfg/ini")).cstr(), "cfg/ini");
    WVPASSEQ(UniConfKey("/").subkey(UniConfKey("cfg/ini")).cstr(), "cfg/ini");
    WVPASSEQ(UniConfKey("cfg").subkey(UniConfKey("cfg/ini")).cstr(), "ini");
    WVPASSEQ(UniConfKey("/cfg/ini").subkey(UniConfKey("cfg/ini")).cstr(), "");
}

WVTEST_MAIN("range")
{
    WVPASSEQ(UniConfKey().range(0,0).cstr(), "");
    WVPASSEQ(UniConfKey().range(0,1).cstr(), "");
    WVPASSEQ(UniConfKey().range(1,2).cstr(), "");
    WVPASSEQ(UniConfKey("").range(0,0).cstr(), "");
    WVPASSEQ(UniConfKey("").range(0,1).cstr(), "");
    WVPASSEQ(UniConfKey("").range(1,2).cstr(), "");
    WVPASSEQ(UniConfKey("fred").range(0,0).cstr(), "");
    WVPASSEQ(UniConfKey("fred").range(0,1).cstr(), "fred");
    WVPASSEQ(UniConfKey("fred").range(1,2).cstr(), "");
    WVPASSEQ(UniConfKey("fred/barney").range(0,0).cstr(), "");
    WVPASSEQ(UniConfKey("fred/barney").range(0,1).cstr(), "fred");
    WVPASSEQ(UniConfKey("fred/barney").range(1,2).cstr(), "barney");
    WVPASSEQ(UniConfKey("fred/barney").range(0,2).cstr(), "fred/barney");
    WVPASSEQ(UniConfKey("fred/barney/betty").range(0,2).cstr(), "fred/barney");
    WVPASSEQ(UniConfKey("fred/barney/betty").range(1,3).cstr(), "barney/betty");
    WVPASSEQ(UniConfKey("fred/barney/betty").range(2,3).cstr(), "betty");
}
