
local moo = import "moo.jsonnet";
local ns = "dunedaq.opmonlib.info";
local s = moo.oschema.schema(ns);

local types = {
    
    varstr : s.string("varstr", moo.re.ident, doc="A variable's name"),

    infobase : s.record('InfoBase', [
            s.field('class_name', self.varstr, "the class name"),
        ]
    ),

    myinfo : s.record('MyInfo', bases=[self.infobase], fields=[
            s.field('another_class_name', self.varstr, "the class name"),
        ]
    ),
};

moo.oschema.sort_select(types, ns)
