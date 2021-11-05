/**
 * @file Issues.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef OPMONLIB_INCLUDE_OPMONLIB_JSONTAGS_HPP_
#define OPMONLIB_INCLUDE_OPMONLIB_JSONTAGS_HPP_

namespace dunedaq {
namespace opmonlib {

struct JSONTags
{
    static inline constexpr char tags[]{ "__tags" }; // parent tags
    static inline constexpr char parent[]{ "__parent" }; // Call it "top"?
    static inline constexpr char time[]{ "__time" };
    static inline constexpr char data[]{ "__data" };
    static inline constexpr char children[]{ "__children" };
    static inline constexpr char properties[]{ "__properties" };
   
};

} // namespace opmonlib
} // namespace dunedaq

#endif // OPMONLIB_INCLUDE_OPMONLIB_JSONTAGS_HPP_
