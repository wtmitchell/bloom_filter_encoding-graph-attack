//===----------------------------------------------------------------------===//
//
// Copyright 2017 Will Mitchell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

#include <boost/graph/graphml.hpp>

#include "bfeattacks/GraphFactory.h"

#include "bloomfilter/BloomFilter.h"
using bloomfilter::BloomFilter;
#include "bloomfilter/HashSet.h"
using bloomfilter::HashSetPair;
#include "bloomfilter/InsertionPolicy.h"
using bloomfilter::InsertionBigramWithSentinel;
using bloomfilter::InsertionTrigramWithSentinel;
#include "hash/HashFactory.h"
#include "graph/Graph.h"

// TODO: Test GraphFactory with other permutations of BloomFilter parameters
TEST(GraphFactory, CalculateVerticesAndEdgesBigram) {
  vector<string> vertices = { "^q", "^t", "bd", "d$", "da", "dq", "ef",
                              "es", "ll", "nu", "q$", "rt", "st", "t$",
                              "te", "uj", "uz", "vn", "we", "wf", "wg",
                              "wl", "yn", "yx", "zm" };

  auto edges = bfeattacks::calculateEdges<2, true, '^', true, '$'>(vertices);

  vector<pair<string, string> > expected_edges = {
    { "Source", "^q" }, { "Source", "^t" }, { "d$", "Sink" }, { "q$", "Sink" },
    { "t$", "Sink" },   { "^q", "q$" },     { "^t", "t$" },   { "^t", "te" },
    { "bd", "d$" },     { "bd", "da" },     { "bd", "dq" },   { "dq", "q$" },
    { "es", "st" },     { "ll", "ll" },     { "nu", "uj" },   { "nu", "uz" },
    { "rt", "t$" },     { "rt", "te" },     { "st", "t$" },   { "st", "te" },
    { "te", "ef" },     { "te", "es" },     { "uz", "zm" },   { "vn", "nu" },
    { "we", "ef" },     { "we", "es" },     { "wl", "ll" },   { "yn", "nu" }
  };
  EXPECT_EQ(expected_edges, edges);

  /*
  auto g = bfeattacks::constructGraph(vertices, edges);

  boost::dynamic_properties dp;
  dp.property("Label", get(&bfeattacks::vertex_info::name, g));
  dp.property("node_id", get(boost::vertex_index, g));

  std::ofstream outf("test.graphml");
  write_graphml(outf, g, dp);
  */
}

TEST(GraphFactory, CalculateVerticesAndEdgesTrigram) {
  vector<string> vertices = {
    "^^c", "^^t", "^ag", "^cj", "^dj", "^em", "^jg", "^jk", "^lj", "^lr", "^mx",
    "^no", "^om", "^qy", "^rx", "^sk", "^sz", "^te", "^ti", "^wm", "^xk", "^zj",
    "^zr", "a$$", "adx", "ady", "aff", "ag$", "age", "agj", "aiz", "ajn", "ajy",
    "akf", "aks", "amd", "aod", "aoq", "aot", "aps", "apu", "aqz", "as$", "asi",
    "asv", "asw", "aut", "auw", "avp", "axu", "aym", "ayt", "azi", "ban", "bbh",
    "bct", "bed", "bes", "bft", "bge", "bju", "bkj", "blw", "bno", "bnu", "bop",
    "brk", "bsb", "bsw", "btz", "bvr", "bxm", "byl", "bzo", "caf", "cal", "cbl",
    "cbs", "ccq", "ccu", "ccw", "cdn", "cdz", "cea", "cfl", "cga", "cgk", "cjs",
    "cjt", "ckp", "cma", "cpj", "cps", "cqr", "cqs", "cqz", "crj", "cry", "cvv",
    "cxm", "cxv", "cya", "cye", "cyw", "dal", "dao", "das", "db$", "deq", "der",
    "det", "df$", "dfi", "dfx", "dgz", "dhy", "dia", "dk$", "dla", "dln", "dnd",
    "dnu", "dpb", "dpz", "dtl", "dvx", "dwm", "dxd", "dyk", "dyz", "dze", "dzr",
    "dzt", "eam", "ebd", "ebk", "eci", "edh", "ehf", "ehi", "ehy", "ekf", "ekl",
    "ekm", "eol", "epe", "eqf", "ert", "esn", "esr", "est", "euy", "euz", "evm",
    "ew$", "exi", "eyw", "fcx", "fdz", "feo", "ffn", "ffu", "fgh", "fgu", "fhf",
    "fkw", "flf", "fli", "flw", "fnj", "fnp", "fnx", "fou", "fqn", "fqq", "fqx",
    "frl", "frp", "fsp", "fst", "fub", "fun", "fvb", "fxp", "fyf", "gaa", "gaq",
    "gbg", "gbu", "gdb", "gep", "gex", "ghk", "gil", "gkz", "gll", "gmb", "gmd",
    "gnm", "gob", "gpa", "gpp", "gqd", "grg", "gur", "gwn", "gxs", "gyg", "gzf",
    "hbd", "hbz", "hcj", "hcm", "hdb", "hdj", "hhn", "hkl", "hko", "hmd", "hmv",
    "hna", "hne", "hnf", "hnz", "hoe", "hoh", "hos", "hq$", "hr$", "hrf", "hrm",
    "htn", "htr", "hul", "huz", "hyn", "hzj", "hzr", "iae", "ibt", "ibv", "icf",
    "ifx", "igc", "igz", "ihl", "iio", "ijf", "ike", "ikm", "ilc", "ilw", "imf",
    "ios", "ip$", "ipa", "ipn", "ipq", "irh", "isf", "iss", "itn", "ium", "ivh",
    "ivq", "iwb", "iyu", "jbx", "jbz", "jci", "jcp", "jdy", "jed", "jep", "jft",
    "jig", "jis", "jjd", "jjg", "jle", "jmr", "jnj", "jnk", "jqp", "jqz", "jsf",
    "jtn", "jvj", "jvv", "jwf", "jwq", "jxj", "jxv", "jzo", "kai", "kc$", "kdd",
    "keo", "kfi", "kft", "kfv", "kfz", "kiw", "kob", "kot", "kqm", "ksk", "kte",
    "ktt", "kzb", "lct", "ldu", "leb", "lfp", "lhq", "lil", "lit", "ljm", "lko",
    "llm", "lma", "lot", "lq$", "lql", "lvr", "lxs", "lxz", "lyb", "m$$", "mao",
    "max", "maz", "mbi", "mcr", "me$", "mfn", "mft", "mgg", "mgx", "mhf", "mim",
    "mix", "mjx", "mkt", "mlr", "mmx", "mtp", "mts", "mul", "mvt", "mwd", "mwv",
    "mxn", "mzf", "nal", "ncu", "ndb", "new", "ngf", "ngg", "ngn", "nhk", "nhp",
    "nih", "nkx", "nmp", "nnc", "nnl", "nnu", "nod", "nqo", "nra", "nvb", "nwb",
    "nx$", "nxg", "nxi", "nxy", "nye", "nyo", "oa$", "odt", "oeq", "oer", "ogd",
    "ogg", "ogq", "ohj", "ohy", "ojo", "olj", "onj", "oob", "oqx", "orj", "orv",
    "osc", "ouh", "ouw", "ovv", "owb", "owt", "owy", "owz", "oxe", "oxw", "oyi",
    "ozb", "pap", "pcd", "pci", "pdz", "pen", "pfn", "pkb", "pkk", "pkz", "pmc",
    "pnm", "pnt", "por", "pqy", "psh", "pth", "ptk", "put", "pv$", "pvq", "pwf",
    "pxy", "qaf", "qbp", "qcm", "qcu", "qcz", "qdl", "qey", "qih", "qio", "qix",
    "qja", "qjk", "qjn", "qjq", "qli", "qlj", "qlx", "qmd", "qno", "qom", "qos",
    "qqn", "qqp", "qra", "qte", "qwo", "qww", "qzk", "qzv", "rbu", "rdg", "rfg",
    "rfu", "rfx", "rgc", "rhk", "rhn", "rii", "rkf", "rkt", "rlf", "rmi", "rne",
    "rnk", "rod", "rph", "rqa", "rqn", "rth", "ruf", "rv$", "rvo", "rwk", "rwq",
    "rxl", "sau", "scn", "sgt", "shn", "shy", "siz", "sjc", "sjw", "slg", "slr",
    "sme", "snz", "spv", "srk", "srv", "ssc", "ssn", "sss", "sst", "st$", "stc",
    "stl", "suf", "sut", "suz", "sv$", "sva", "svi", "svv", "svz", "sye", "t$$",
    "tbm", "tca", "tct", "tes", "tfe", "tfm", "tg$", "tgn", "tiz", "tkd", "tla",
    "tms", "tot", "toy", "tpd", "tqt", "tst", "tvs", "tvu", "tvw", "txg", "uab",
    "ubv", "ucp", "udl", "udp", "ue$", "ueo", "uie", "ujw", "ujy", "uko", "ukq",
    "ulp", "umv", "unw", "uo$", "uol", "uqx", "urn", "uro", "ust", "utg", "uug",
    "uus", "uwz", "uxi", "uyo", "uzh", "uzq", "vba", "vbz", "vch", "vek", "vex",
    "vgd", "vgr", "vgv", "vgx", "vkn", "vlp", "vlq", "vlv", "vnh", "vnj", "von",
    "vtd", "vtk", "vvb", "vvf", "vvv", "vwk", "vwr", "wal", "wcq", "wfp", "wgj",
    "whb", "wjr", "wrs", "wsr", "wst", "wsu", "wtu", "wup", "wvn", "wwg", "wwt",
    "wxl", "wxx", "wzm", "wzy", "xcl", "xcp", "xde", "xdk", "xeb", "xey", "xfq",
    "xgb", "xgf", "xgt", "xhi", "xhn", "xis", "xk$", "xkk", "xmi", "xnl", "xog",
    "xq$", "xsk", "xtl", "xts", "xtv", "xui", "xwq", "xws", "xxc", "yba", "ybq",
    "yct", "ydh", "ydl", "yed", "ygs", "yje", "ylv", "yma", "ymn", "you", "yqh",
    "yro", "ytf", "ytl", "ytz", "yur", "yw$", "ywt", "yxw", "yya", "yyi", "yyy",
    "zab", "zaz", "zby", "zcu", "zeq", "zfj", "zje", "zlc", "zoc", "zol", "zoz",
    "zpi", "zpu", "zpw", "zqh", "zqs", "zqx", "zrz", "zto", "zud", "zvf", "zvw",
    "zyj", "zyx", "zyy"
  };

  auto edges = bfeattacks::calculateEdges<3, true, '^', true, '$'>(vertices);

  vector<pair<string, string> > expected_edges = {
    { "Source", "^^c" }, { "Source", "^^t" }, { "a$$", "Sink" },
    { "m$$", "Sink" },   { "t$$", "Sink" },   { "^^c", "^cj" },
    { "^^t", "^te" },    { "^^t", "^ti" },    { "^ag", "ag$" },
    { "^ag", "age" },    { "^ag", "agj" },    { "^cj", "cjs" },
    { "^cj", "cjt" },    { "^lj", "ljm" },    { "^mx", "mxn" },
    { "^no", "nod" },    { "^rx", "rxl" },    { "^te", "tes" },
    { "^ti", "tiz" },    { "^xk", "xk$" },    { "^xk", "xkk" },
    { "^zj", "zje" },    { "^zr", "zrz" },    { "adx", "dxd" },
    { "ady", "dyk" },    { "ady", "dyz" },    { "aff", "ffn" },
    { "aff", "ffu" },    { "age", "gep" },    { "age", "gex" },
    { "ajn", "jnj" },    { "ajn", "jnk" },    { "akf", "kfi" },
    { "akf", "kft" },    { "akf", "kfv" },    { "akf", "kfz" },
    { "aks", "ksk" },    { "aod", "odt" },    { "aoq", "oqx" },
    { "aps", "psh" },    { "apu", "put" },    { "aqz", "qzk" },
    { "aqz", "qzv" },    { "asi", "siz" },    { "asv", "sv$" },
    { "asv", "sva" },    { "asv", "svi" },    { "asv", "svv" },
    { "asv", "svz" },    { "aut", "utg" },    { "auw", "uwz" },
    { "axu", "xui" },    { "aym", "yma" },    { "aym", "ymn" },
    { "ayt", "ytf" },    { "ayt", "ytl" },    { "ayt", "ytz" },
    { "bed", "edh" },    { "bes", "esn" },    { "bes", "esr" },
    { "bes", "est" },    { "bge", "gep" },    { "bge", "gex" },
    { "bno", "nod" },    { "brk", "rkf" },    { "brk", "rkt" },
    { "bxm", "xmi" },    { "byl", "ylv" },    { "bzo", "zoc" },
    { "bzo", "zol" },    { "bzo", "zoz" },    { "caf", "aff" },
    { "cbl", "blw" },    { "cbs", "bsb" },    { "cbs", "bsw" },
    { "ccq", "cqr" },    { "ccq", "cqs" },    { "ccq", "cqz" },
    { "cdn", "dnd" },    { "cdn", "dnu" },    { "cdz", "dze" },
    { "cdz", "dzr" },    { "cdz", "dzt" },    { "cea", "eam" },
    { "cfl", "flf" },    { "cfl", "fli" },    { "cfl", "flw" },
    { "cga", "gaa" },    { "cga", "gaq" },    { "cgk", "gkz" },
    { "cjs", "jsf" },    { "cjt", "jtn" },    { "cma", "mao" },
    { "cma", "max" },    { "cma", "maz" },    { "cps", "psh" },
    { "cqr", "qra" },    { "cqz", "qzk" },    { "cqz", "qzv" },
    { "cvv", "vvb" },    { "cvv", "vvf" },    { "cvv", "vvv" },
    { "cxm", "xmi" },    { "cye", "yed" },    { "cyw", "yw$" },
    { "cyw", "ywt" },    { "dao", "aod" },    { "dao", "aoq" },
    { "dao", "aot" },    { "das", "as$" },    { "das", "asi" },
    { "das", "asv" },    { "das", "asw" },    { "deq", "eqf" },
    { "der", "ert" },    { "dfx", "fxp" },    { "dgz", "gzf" },
    { "dhy", "hyn" },    { "dia", "iae" },    { "dnd", "ndb" },
    { "dtl", "tla" },    { "dxd", "xde" },    { "dxd", "xdk" },
    { "dze", "zeq" },    { "dzr", "zrz" },    { "dzt", "zto" },
    { "eam", "amd" },    { "ebk", "bkj" },    { "edh", "dhy" },
    { "ehy", "hyn" },    { "ekf", "kfi" },    { "ekf", "kft" },
    { "ekf", "kfv" },    { "ekf", "kfz" },    { "eol", "olj" },
    { "epe", "pen" },    { "ert", "rth" },    { "esn", "snz" },
    { "esr", "srk" },    { "esr", "srv" },    { "est", "st$" },
    { "est", "stc" },    { "est", "stl" },    { "euy", "uyo" },
    { "euz", "uzh" },    { "euz", "uzq" },    { "exi", "xis" },
    { "eyw", "yw$" },    { "eyw", "ywt" },    { "fcx", "cxm" },
    { "fcx", "cxv" },    { "fdz", "dze" },    { "fdz", "dzr" },
    { "fdz", "dzt" },    { "feo", "eol" },    { "ffn", "fnj" },
    { "ffn", "fnp" },    { "ffn", "fnx" },    { "ffu", "fub" },
    { "ffu", "fun" },    { "fgh", "ghk" },    { "fgu", "gur" },
    { "flf", "lfp" },    { "fli", "lil" },    { "fli", "lit" },
    { "fnx", "nx$" },    { "fnx", "nxg" },    { "fnx", "nxi" },
    { "fnx", "nxy" },    { "fou", "ouh" },    { "fou", "ouw" },
    { "fqn", "qno" },    { "fqq", "qqn" },    { "fqq", "qqp" },
    { "frl", "rlf" },    { "frp", "rph" },    { "fsp", "spv" },
    { "fst", "st$" },    { "fst", "stc" },    { "fst", "stl" },
    { "fub", "ubv" },    { "fun", "unw" },    { "fvb", "vba" },
    { "fvb", "vbz" },    { "gaq", "aqz" },    { "gbg", "bge" },
    { "gdb", "db$" },    { "gep", "epe" },    { "gex", "exi" },
    { "ghk", "hkl" },    { "ghk", "hko" },    { "gil", "ilc" },
    { "gil", "ilw" },    { "gkz", "kzb" },    { "gll", "llm" },
    { "gmb", "mbi" },    { "gnm", "nmp" },    { "gpa", "pap" },
    { "gqd", "qdl" },    { "grg", "rgc" },    { "gur", "urn" },
    { "gur", "uro" },    { "gxs", "xsk" },    { "gyg", "ygs" },
    { "gzf", "zfj" },    { "hbz", "bzo" },    { "hcj", "cjs" },
    { "hcj", "cjt" },    { "hcm", "cma" },    { "hdb", "db$" },
    { "hhn", "hna" },    { "hhn", "hne" },    { "hhn", "hnf" },
    { "hhn", "hnz" },    { "hko", "kob" },    { "hko", "kot" },
    { "hmv", "mvt" },    { "hna", "nal" },    { "hne", "new" },
    { "hoe", "oeq" },    { "hoe", "oer" },    { "hoh", "ohj" },
    { "hoh", "ohy" },    { "hos", "osc" },    { "hrf", "rfg" },
    { "hrf", "rfu" },    { "hrf", "rfx" },    { "hrm", "rmi" },
    { "hul", "ulp" },    { "huz", "uzh" },    { "huz", "uzq" },
    { "hzj", "zje" },    { "hzr", "zrz" },    { "ibt", "btz" },
    { "ibv", "bvr" },    { "icf", "cfl" },    { "ifx", "fxp" },
    { "igz", "gzf" },    { "iio", "ios" },    { "ijf", "jft" },
    { "ike", "keo" },    { "ilc", "lct" },    { "imf", "mfn" },
    { "imf", "mft" },    { "ios", "osc" },    { "ipa", "pap" },
    { "ipn", "pnm" },    { "ipn", "pnt" },    { "ipq", "pqy" },
    { "irh", "rhk" },    { "irh", "rhn" },    { "iss", "ssc" },
    { "iss", "ssn" },    { "iss", "sss" },    { "iss", "sst" },
    { "ium", "umv" },    { "iyu", "yur" },    { "jbx", "bxm" },
    { "jbz", "bzo" },    { "jcp", "cpj" },    { "jcp", "cps" },
    { "jdy", "dyk" },    { "jdy", "dyz" },    { "jed", "edh" },
    { "jep", "epe" },    { "jig", "igc" },    { "jig", "igz" },
    { "jis", "isf" },    { "jis", "iss" },    { "jjd", "jdy" },
    { "jle", "leb" },    { "jnk", "nkx" },    { "jqz", "qzk" },
    { "jqz", "qzv" },    { "jvv", "vvb" },    { "jvv", "vvf" },
    { "jvv", "vvv" },    { "jwf", "wfp" },    { "jzo", "zoc" },
    { "jzo", "zol" },    { "jzo", "zoz" },    { "kai", "aiz" },
    { "keo", "eol" },    { "kfv", "fvb" },    { "kiw", "iwb" },
    { "kqm", "qmd" },    { "kte", "tes" },    { "kzb", "zby" },
    { "leb", "ebd" },    { "leb", "ebk" },    { "lhq", "hq$" },
    { "lil", "ilc" },    { "lil", "ilw" },    { "lit", "itn" },
    { "ljm", "jmr" },    { "lko", "kob" },    { "lko", "kot" },
    { "llm", "lma" },    { "lma", "mao" },    { "lma", "max" },
    { "lma", "maz" },    { "lql", "qli" },    { "lql", "qlj" },
    { "lql", "qlx" },    { "lxs", "xsk" },    { "lyb", "yba" },
    { "lyb", "ybq" },    { "mao", "aod" },    { "mao", "aoq" },
    { "mao", "aot" },    { "max", "axu" },    { "maz", "azi" },
    { "mcr", "crj" },    { "mcr", "cry" },    { "mfn", "fnj" },
    { "mfn", "fnp" },    { "mfn", "fnx" },    { "mgx", "gxs" },
    { "mim", "imf" },    { "mjx", "jxj" },    { "mjx", "jxv" },
    { "mkt", "kte" },    { "mkt", "ktt" },    { "mmx", "mxn" },
    { "mtp", "tpd" },    { "mts", "tst" },    { "mul", "ulp" },
    { "mvt", "vtd" },    { "mvt", "vtk" },    { "mwv", "wvn" },
    { "mxn", "xnl" },    { "mzf", "zfj" },    { "ndb", "db$" },
    { "new", "ew$" },    { "ngn", "gnm" },    { "nhk", "hkl" },
    { "nhk", "hko" },    { "nih", "ihl" },    { "nnc", "ncu" },
    { "nod", "odt" },    { "nqo", "qom" },    { "nqo", "qos" },
    { "nvb", "vba" },    { "nvb", "vbz" },    { "nxg", "xgb" },
    { "nxg", "xgf" },    { "nxg", "xgt" },    { "nxi", "xis" },
    { "nye", "yed" },    { "nyo", "you" },    { "oa$", "a$$" },
    { "odt", "dtl" },    { "oeq", "eqf" },    { "oer", "ert" },
    { "ogd", "gdb" },    { "ogq", "gqd" },    { "ohy", "hyn" },
    { "olj", "ljm" },    { "orv", "rv$" },    { "orv", "rvo" },
    { "osc", "scn" },    { "ouw", "uwz" },    { "ovv", "vvb" },
    { "ovv", "vvf" },    { "ovv", "vvv" },    { "owt", "wtu" },
    { "owz", "wzm" },    { "owz", "wzy" },    { "oxe", "xeb" },
    { "oxe", "xey" },    { "oxw", "xwq" },    { "oxw", "xws" },
    { "ozb", "zby" },    { "pap", "aps" },    { "pap", "apu" },
    { "pcd", "cdn" },    { "pcd", "cdz" },    { "pdz", "dze" },
    { "pdz", "dzr" },    { "pdz", "dzt" },    { "pfn", "fnj" },
    { "pfn", "fnp" },    { "pfn", "fnx" },    { "pkz", "kzb" },
    { "pmc", "mcr" },    { "pnm", "nmp" },    { "por", "orj" },
    { "por", "orv" },    { "psh", "shn" },    { "psh", "shy" },
    { "ptk", "tkd" },    { "put", "utg" },    { "pwf", "wfp" },
    { "qaf", "aff" },    { "qcm", "cma" },    { "qdl", "dla" },
    { "qdl", "dln" },    { "qey", "eyw" },    { "qih", "ihl" },
    { "qio", "ios" },    { "qjn", "jnj" },    { "qjn", "jnk" },
    { "qjq", "jqp" },    { "qjq", "jqz" },    { "qli", "lil" },
    { "qli", "lit" },    { "qlj", "ljm" },    { "qlx", "lxs" },
    { "qlx", "lxz" },    { "qno", "nod" },    { "qos", "osc" },
    { "qqn", "qno" },    { "qte", "tes" },    { "qww", "wwg" },
    { "qww", "wwt" },    { "qzv", "zvf" },    { "qzv", "zvw" },
    { "rdg", "dgz" },    { "rfg", "fgh" },    { "rfg", "fgu" },
    { "rfu", "fub" },    { "rfu", "fun" },    { "rfx", "fxp" },
    { "rhk", "hkl" },    { "rhk", "hko" },    { "rhn", "hna" },
    { "rhn", "hne" },    { "rhn", "hnf" },    { "rhn", "hnz" },
    { "rii", "iio" },    { "rkf", "kfi" },    { "rkf", "kft" },
    { "rkf", "kfv" },    { "rkf", "kfz" },    { "rkt", "kte" },
    { "rkt", "ktt" },    { "rlf", "lfp" },    { "rmi", "mim" },
    { "rmi", "mix" },    { "rne", "new" },    { "rnk", "nkx" },
    { "rod", "odt" },    { "rqa", "qaf" },    { "rqn", "qno" },
    { "rvo", "von" },    { "sau", "aut" },    { "sau", "auw" },
    { "shn", "hna" },    { "shn", "hne" },    { "shn", "hnf" },
    { "shn", "hnz" },    { "shy", "hyn" },    { "sjc", "jci" },
    { "sjc", "jcp" },    { "sjw", "jwf" },    { "sjw", "jwq" },
    { "sme", "me$" },    { "spv", "pv$" },    { "spv", "pvq" },
    { "srk", "rkf" },    { "srk", "rkt" },    { "srv", "rv$" },
    { "srv", "rvo" },    { "ssc", "scn" },    { "ssn", "snz" },
    { "sss", "ssc" },    { "sss", "ssn" },    { "sss", "sss" },
    { "sss", "sst" },    { "sst", "st$" },    { "sst", "stc" },
    { "sst", "stl" },    { "st$", "t$$" },    { "stc", "tca" },
    { "stc", "tct" },    { "stl", "tla" },    { "sut", "utg" },
    { "suz", "uzh" },    { "suz", "uzq" },    { "svv", "vvb" },
    { "svv", "vvf" },    { "svv", "vvv" },    { "sye", "yed" },
    { "tca", "caf" },    { "tca", "cal" },    { "tes", "esn" },
    { "tes", "esr" },    { "tes", "est" },    { "tfe", "feo" },
    { "tgn", "gnm" },    { "tkd", "kdd" },    { "toy", "oyi" },
    { "tpd", "pdz" },    { "tqt", "qte" },    { "tst", "st$" },
    { "tst", "stc" },    { "tst", "stl" },    { "tvw", "vwk" },
    { "tvw", "vwr" },    { "txg", "xgb" },    { "txg", "xgf" },
    { "txg", "xgt" },    { "ubv", "bvr" },    { "ucp", "cpj" },
    { "ucp", "cps" },    { "udl", "dla" },    { "udl", "dln" },
    { "udp", "dpb" },    { "udp", "dpz" },    { "ueo", "eol" },
    { "ujw", "jwf" },    { "ujw", "jwq" },    { "uko", "kob" },
    { "uko", "kot" },    { "ukq", "kqm" },    { "umv", "mvt" },
    { "unw", "nwb" },    { "uol", "olj" },    { "urn", "rne" },
    { "urn", "rnk" },    { "uro", "rod" },    { "ust", "st$" },
    { "ust", "stc" },    { "ust", "stl" },    { "utg", "tg$" },
    { "utg", "tgn" },    { "uus", "ust" },    { "uwz", "wzm" },
    { "uwz", "wzy" },    { "uxi", "xis" },    { "uyo", "you" },
    { "uzq", "zqh" },    { "uzq", "zqs" },    { "uzq", "zqx" },
    { "vba", "ban" },    { "vbz", "bzo" },    { "vek", "ekf" },
    { "vek", "ekl" },    { "vek", "ekm" },    { "vex", "exi" },
    { "vgd", "gdb" },    { "vgr", "grg" },    { "vgx", "gxs" },
    { "vlq", "lq$" },    { "vlq", "lql" },    { "vlv", "lvr" },
    { "vnh", "nhk" },    { "vnh", "nhp" },    { "von", "onj" },
    { "vtk", "tkd" },    { "vvb", "vba" },    { "vvb", "vbz" },
    { "vvv", "vvb" },    { "vvv", "vvf" },    { "vvv", "vvv" },
    { "vwr", "wrs" },    { "wcq", "cqr" },    { "wcq", "cqs" },
    { "wcq", "cqz" },    { "whb", "hbd" },    { "whb", "hbz" },
    { "wsr", "srk" },    { "wsr", "srv" },    { "wst", "st$" },
    { "wst", "stc" },    { "wst", "stl" },    { "wsu", "suf" },
    { "wsu", "sut" },    { "wsu", "suz" },    { "wvn", "vnh" },
    { "wvn", "vnj" },    { "wwg", "wgj" },    { "wwt", "wtu" },
    { "wxx", "xxc" },    { "wzy", "zyj" },    { "wzy", "zyx" },
    { "wzy", "zyy" },    { "xcp", "cpj" },    { "xcp", "cps" },
    { "xde", "deq" },    { "xde", "der" },    { "xde", "det" },
    { "xdk", "dk$" },    { "xeb", "ebd" },    { "xeb", "ebk" },
    { "xey", "eyw" },    { "xfq", "fqn" },    { "xfq", "fqq" },
    { "xfq", "fqx" },    { "xgb", "gbg" },    { "xgb", "gbu" },
    { "xhn", "hna" },    { "xhn", "hne" },    { "xhn", "hnf" },
    { "xhn", "hnz" },    { "xis", "isf" },    { "xis", "iss" },
    { "xmi", "mim" },    { "xmi", "mix" },    { "xog", "ogd" },
    { "xog", "ogg" },    { "xog", "ogq" },    { "xtl", "tla" },
    { "xts", "tst" },    { "xtv", "tvs" },    { "xtv", "tvu" },
    { "xtv", "tvw" },    { "xui", "uie" },    { "xws", "wsr" },
    { "xws", "wst" },    { "xws", "wsu" },    { "xxc", "xcl" },
    { "xxc", "xcp" },    { "yba", "ban" },    { "ydh", "dhy" },
    { "ydl", "dla" },    { "ydl", "dln" },    { "yed", "edh" },
    { "yje", "jed" },    { "yje", "jep" },    { "ylv", "lvr" },
    { "yma", "mao" },    { "yma", "max" },    { "yma", "maz" },
    { "you", "ouh" },    { "you", "ouw" },    { "yro", "rod" },
    { "ytf", "tfe" },    { "ytf", "tfm" },    { "ytl", "tla" },
    { "yur", "urn" },    { "yur", "uro" },    { "ywt", "wtu" },
    { "yxw", "xwq" },    { "yxw", "xws" },    { "yyy", "yya" },
    { "yyy", "yyi" },    { "yyy", "yyy" },    { "zaz", "azi" },
    { "zby", "byl" },    { "zeq", "eqf" },    { "zje", "jed" },
    { "zje", "jep" },    { "zlc", "lct" },    { "zol", "olj" },
    { "zoz", "ozb" },    { "zpu", "put" },    { "zpw", "pwf" },
    { "zto", "tot" },    { "zto", "toy" },    { "zud", "udl" },
    { "zud", "udp" },    { "zvw", "vwk" },    { "zvw", "vwr" },
    { "zyj", "yje" },    { "zyx", "yxw" },    { "zyy", "yya" },
    { "zyy", "yyi" },    { "zyy", "yyy" }
  };
  EXPECT_EQ(expected_edges, edges);

  auto g = bfeattacks::constructGraph(vertices, edges);

  // boost::dynamic_properties dp;
  // dp.property("Label", get(&graph::vertex_info::name, g));
  // dp.property("node_id", get(boost::vertex_index, g));

  // std::ofstream outf("test.graphml");
  // write_graphml(outf, g, dp);
}

TEST(GraphFactory, CalculateVerticesAndEdgesTrigramRamakrishna) {
  // From ramakrishna m = 256 Hashes: Pair (k = 10) {MD5, SHA-3-256}
  // Insertion policy: 3-gram with start sentinel '^' and with stop sentinel '$'
  vector<string> vertices = {
    "^^r", "^kr", "^pw", "^ra", "^sp", "^vp", "^we", "a$$", "akr", "ama",
    "apf", "att", "boz", "cca", "cpi", "ctm", "djh", "do$", "gfb", "ghm",
    "gve", "hdi", "hna", "imu", "ish", "iuq", "ivd", "iww", "jyg", "kmy",
    "kri", "lcu", "llv", "lsg", "lwq", "lyk", "mak", "mcl", "mra", "msy",
    "muz", "mye", "na$", "nef", "nk$", "nor", "oft", "oss", "osy", "oxb",
    "pat", "pco", "plm", "pqn", "qge", "qxt", "ram", "ris", "shn", "sqs",
    "tye", "uhc", "vq$", "vxf", "wlz", "xka", "yfs", "zcw", "znk"
  };

  auto edges = bfeattacks::calculateEdges<3, true, '^', true, '$'>(vertices);

  vector<pair<string, string> > expected_edges =
      { { "Source", "^^r" }, { "a$$", "Sink" }, { "^^r", "^ra" },
        { "^kr", "kri" },    { "^ra", "ram" },  { "akr", "kri" },
        { "ama", "mak" },    { "hna", "na$" },  { "imu", "muz" },
        { "ish", "shn" },    { "kmy", "mye" },  { "kri", "ris" },
        { "mak", "akr" },    { "mra", "ram" },  { "na$", "a$$" },
        { "pat", "att" },    { "ram", "ama" },  { "ris", "ish" },
        { "shn", "hna" },    { "znk", "nk$" } };
  EXPECT_EQ(expected_edges, edges);

  auto g = bfeattacks::constructGraph(vertices, edges);

  // boost::dynamic_properties dp;
  // dp.property("Label", get(&graph::vertex_info::name, g));
  // dp.property("node_id", get(boost::vertex_index, g));

  // std::ofstream outf("test.graphml");
  // write_graphml(outf, g, dp);
}

TEST(GraphFactory, ConstructGraphBigram) {
  HashSetPair hs(5);

  hs.add(hash::MD5).add(hash::SHA3_256);

  BloomFilter<HashSetPair, InsertionBigramWithSentinel, true> bf(50, hs);

  bf.insert("test");

  vector<string> potential_members =
      bf.potential_members("abcdefghijklmnopqrstuvwxyz");

  bfeattacks::constructGraph(bf, "abcdefghijklmnopqrstuvwxyz");
}

/*
TEST(GraphFactory, ConstructGraphTrigram) {
  HashSetPair hs(5);
  MD5 md5;
  SHA3_256 sha3;

  hs.add(&md5).add(&sha3);

  BloomFilter<HashSetPair, InsertionTrigramWithSentinel, true> bf(50, hs);

  bf.insert("test");

  vector<string> potential_members =
      bf.potential_members("abcdefghijklmnopqrstuvwxyz");

  for (const auto& i : potential_members)
    cout << i << " ";
  cout << endl;

  bfeattacks::constructGraph(bf, "abcdefghijklmnopqrstuvwxyz");
}
*/

TEST(GraphFactory, ConstructGraphSSN) {
  HashSetPair hs(30);

  hs.add(hash::MD5).add(hash::SHA3_256);

  BloomFilter<HashSetPair, InsertionBigramWithSentinel, true> bf(1000, hs);

  bf.insert("123456789");

  vector<string> potential_members = bf.potential_members("1234567890");

  vector<string> expected_vertices = { "^1", "12", "23", "34", "45", "56",
                                       "67", "78", "89", "9$" };
  EXPECT_EQ(expected_vertices, potential_members);

  auto edges =
      bfeattacks::calculateEdges<2, true, '^', true, '$'>(potential_members);

  vector<pair<string, string>> expected_edges = {
      {"Source", "^1"}, {"9$", "Sink"}, {"12", "23"}, {"23", "34"},
      {"34", "45"},     {"45", "56"},   {"56", "67"}, {"67", "78"},
      {"78", "89"},     {"89", "9$"},   {"^1", "12"}};
  EXPECT_EQ(expected_edges, edges);

  auto g = bfeattacks::constructGraph(expected_vertices, expected_edges);

  boost::dynamic_properties dp;
  dp.property("Label", get(&graph::vertex_info::name, g));
  dp.property("node_id", get(boost::vertex_index, g));

  //std::ofstream outf("test.graphml");
  //write_graphml(outf, g, dp);
}
