#include <tiledb/tiledb> // C++

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

namespace libtiledbcpp {

using namespace tiledb;
namespace py = pybind11;

template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

//template <typename T>
//declare_dimension

void init_enums(py::module&);
void init_domain(py::module& m);

PYBIND11_MODULE(cc, m) {
  //py::enum_<tiledb_datatype_t>(m, "tiledb_datatype");

  init_enums(m);
  init_domain(m);

  py::class_<Context>(m, "Context")
    .def(py::init())
    .def(py::init<Config>())
    .def_property_readonly("config", &Context::config);

  py::class_<tiledb::Config>(m, "Config")
    .def(py::init())
    .def(py::init<std::map<std::string, std::string>>())
    .def(py::init<std::string>())
    .def("set", &Config::set)
    .def("get", &Config::get)
    .def("save_to_file", &Config::save_to_file)
    .def("__eq__", &Config::operator==)
    .def("__ne__", &Config::operator!=)
    //.def("_ptr", &Config::ptr) // TBD should this be capsule?
    .def("__setitem__", [](Config& cfg, std::string &param, std::string &val) {
       cfg[param] = val;
    })
    .def("__getitem__", [](const Config& cfg, std::string &param) {
        try {
          return cfg.get(param);
        } catch(TileDBError &e) {
          throw py::key_error();
        }
    })
    .def("__delitem__", [](Config& cfg, const std::string& param) {
      try {
        cfg.unset(param);
      } catch(TileDBError &e) {
        throw py::key_error();
      }
    })
    .def("__iter__", [](Config& cfg) {
        return py::make_iterator(cfg.begin(), cfg.end());
    }, py::keep_alive<0,1>())
    .def("__del__", &Config::unset)
    .def("unset", &Config::unset);

  /*
  py::class_<tiledb::Domain>(m, "Domain")
    .def(py::init<Context>())
    .def("cell_num", [](Domain& dom) { return dom.cell_num(); })
    //.def("dump")
    .def("tiledb_datatype", &Domain::type)
    .def("ndim", &Domain::ndim)
    .def("dimensions", &Domain::dimensions)
    .def("dimension", py::overload_cast<const std::string&>(&Domain::dimension, py::const_))
    .def("dimension", py::overload_cast<const std::string&>(&Domain::dimension, py::const_))
    .def("add_dimension", &Domain::add_dimension);
  */

}


/*
PYBIND11_MODULE(libtiledbcpp, m) {
  py::class_<tiledb::Query>(m, "Query")
      .def(py::init<py::object, py::object, py::iterable, py::object,
                    py::object, py::object>())
      .def("set_ranges", &PyQuery::set_ranges)
      .def("set_subarray", &PyQuery::set_subarray)
*/

}; // namespace libtiledbcpp