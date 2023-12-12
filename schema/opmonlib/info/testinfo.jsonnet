// This is a test  info schema used by opmonlib for its (unit)tests.
// It's just a collection of different types of data

local moo = import "moo.jsonnet";
local s = moo.oschema.schema("dunedaq.opmonlib.testinfo");

local info = {
   counter  : s.number("int8", "i8", doc="An signed integer of 8 bytes"),
   rate     : s.number("float8", "f8", doc="A signed float of 8 bytes"),
   string_t : s.string("string_t", moo.re.ident,  doc="A string field"),
   bool_t   : s.boolean("bool_t", doc="boolean field"),

   info: s.record("Info", [
       s.field("integer_example", self.counter, 0, doc="example of integer"),
       s.field("float_example", self.rate, 0, doc="example of floating"),
       s.field("string_example", self.string_t, doc="example of string"),
       s.field("bool_example", self.bool_t, 0, doc="example of bool")

   ], doc="Test info")
};

moo.oschema.sort_select(info)