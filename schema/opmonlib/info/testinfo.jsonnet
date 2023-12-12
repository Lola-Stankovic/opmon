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
       s.field("capacity_rate", self.rate, 0, doc="Estimated TR rate that the application can generate, in Hz"),
       s.field("outstanding_decisions", self.counter, 0, doc="Decisions currently in progress"),	 
       s.field("completed_trigger_records", self.counter, 0, doc="Number of completed TR"),
       s.field("min_completion_time", self.counter, 0, doc="Minimum time (us) for decision to complete"),
       s.field("max_completion_time", self.counter, 0, doc="Maximum time (us) for decision to complete"),
       s.field("waiting_time", self.counter, 0, doc="cumulative time (microseconds) for decisions to be completed"),
       s.field("total_time_since_assignment", self.counter, 0, doc="total time since assignment for all current TDs (ms)"),
       s.field("min_time_since_assignment", self.counter, 0, doc="shortest time since assignment among current TDs (ms)"),
       s.field("max_time_since_assignment", self.counter, 0, doc="longest time since assignment among current TDs (ms)")

   ], doc="Data Flow Application information")
};

moo.oschema.sort_select(info)