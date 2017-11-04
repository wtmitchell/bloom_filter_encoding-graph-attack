//===-- graph/Traversals.cpp - Various graph traversals --------*- C++ -*--===//
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
//
// This file contains some graph traversals
//
// Large portions of this file are copied and modified from
// <boost/graph/depth_first_search.hpp>
//
//===----------------------------------------------------------------------===//

#include "graph/Traversals.h"

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>

#include "graph/Graph.h"
using graph::Graph_t;
using graph::Edge_t;
using graph::Vertex_t;

namespace {
class targeted_path_visitor : public boost::default_dfs_visitor {
public:
  targeted_path_visitor(const Vertex_t stop_,
                        std::vector<std::vector<std::string> > &paths)
      : branch(), found_paths(paths), stop(stop_) {}

  // void back_edge(Edge e, const Graph& g) const {}
  void examine_edge(Edge_t e, const Graph_t &g) {
    // cout << "examine_edge: (" << g[source(e,g)].name << ", " << g[target(e,
    // g)].name << ")\n";
    if (target(e, g) != stop)
      return;

    branch.push_back(g[stop].name);

    // cout << "--- path: ";
    // for (const auto& i : branch)
    //   cout << i << " ";
    // cout << endl;

    found_paths.push_back(branch);
    branch.pop_back();
  }
  // void forward_or_cross_edge(Edge e, const Graph& g) const {}
  // void tree_edge(Edge e, const Graph& g) const {}

  void discover_vertex(Vertex_t v, const Graph_t &g) {
    // cout << "discover_vertex " << g[v].name;

    // cout << " branch: ";
    // for (const auto& i : branch)
    //   cout << i << " ";
    // cout << "\n";
    branch.push_back(g[v].name);
  }
  void finish_vertex(Vertex_t /*v*/, const Graph_t & /*g*/) {
    // cout << "finish_vertex " << g[v].name;
    // cout << " branch: ";
    // for (const auto& i : branch)
    //   cout << i << " ";
    // cout << "\n";
    branch.pop_back();
  }
  // void initialize_vertex(Vertex v, const Graph& g) const {}
  // void start_vertex(Vertex v, const Graph& g) {}

private:
  std::vector<std::string> branch;
  std::vector<std::vector<std::string> > &found_paths;
  const Vertex_t stop;
};

template <class VertexListGraph, class DFSVisitor>
void
all_edge_disjoint_paths(const VertexListGraph &g, DFSVisitor vis,
                        typename boost::graph_traits<
                            VertexListGraph>::vertex_descriptor start_vertex) {
  typedef typename boost::graph_traits<VertexListGraph>::vertex_descriptor
  Vertex;
  typedef typename boost::graph_traits<VertexListGraph>::edge_descriptor Edge;
  BOOST_CONCEPT_ASSERT((boost::DFSVisitorConcept<DFSVisitor, VertexListGraph>));

  typedef typename boost::graph_traits<VertexListGraph>::out_edge_iterator
  EdgeIter;
  typedef std::pair<Vertex, std::pair<EdgeIter, EdgeIter> > VertexInfo;

  // Get the internal property map which indexes the edges
  auto edgeNum = get(boost::edge_index, g);

  // Set all the edge colors to initially be white
  enum Color {
    black,
    white
  };
  std::vector<Color> edgeColors(num_edges(g), white);

  // Start actual traversal
  vis.start_vertex(start_vertex, g);

  std::vector<VertexInfo> stack;
  std::vector<size_t> edges_taken;
  EdgeIter ei, ei_end;
  Vertex current = start_vertex;

  boost::tie(ei, ei_end) = out_edges(current, g);
  stack.push_back(std::make_pair(current, std::make_pair(ei, ei_end)));
  vis.discover_vertex(current, g);

  while (!stack.empty()) {
    VertexInfo &back = stack.back();
    current = back.first;
    boost::tie(ei, ei_end) = back.second;
    stack.pop_back();

    while (ei != ei_end) {
      Vertex next = target(*ei, g);
      vis.examine_edge(*ei, g);
      // Check if edge can be taken
      if (edgeColors[edgeNum[*ei]] == white) {
        // New edge, take it and store current status to return to later
        edges_taken.push_back(edgeNum[*ei]);
        edgeColors[edgeNum[*ei]] = black;
        stack.push_back(std::make_pair(current, std::make_pair(++ei, ei_end)));
        current = next;
        vis.discover_vertex(current, g);
        boost::tie(ei, ei_end) = out_edges(current, g);
      } else {
        // Already seen this edge
        ++ei;
      }
    }
    if (!edges_taken.empty()) {
      // Finished exploring a given edge
      edgeColors[edges_taken.back()] = white;
      edges_taken.pop_back();
    }
    vis.finish_vertex(current, g);
  }
}

// Simple modification of boost::detail::depth_first_visit_impl from
// boost/graph/depth_first_search.hpp
template <class IncidenceGraph, class DFSVisitor, class ColorMap,
          class TerminatorFunc>
void all_simple_paths_impl(
    const IncidenceGraph &g,
    typename boost::graph_traits<IncidenceGraph>::vertex_descriptor u,
    DFSVisitor &vis, ColorMap color, TerminatorFunc func = TerminatorFunc()) {
  BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<IncidenceGraph>));
  BOOST_CONCEPT_ASSERT((boost::DFSVisitorConcept<DFSVisitor, IncidenceGraph>));
  typedef typename boost::graph_traits<IncidenceGraph>::vertex_descriptor
  Vertex;
  BOOST_CONCEPT_ASSERT(
      (boost::ReadWritePropertyMapConcept<ColorMap, Vertex_t>));
  typedef typename boost::property_traits<ColorMap>::value_type ColorValue;
  BOOST_CONCEPT_ASSERT((boost::ColorValueConcept<ColorValue>));
  typedef boost::color_traits<ColorValue> Color;
  typedef typename boost::graph_traits<IncidenceGraph>::out_edge_iterator Iter;
  typedef std::pair<Vertex_t, std::pair<Iter, Iter> > VertexInfo;

  Iter ei, ei_end;
  std::vector<VertexInfo> stack;

  // Possible optimization for vector
  // stack.reserve(num_vertices(g));

  typedef typename boost::unwrap_reference<TerminatorFunc>::type TF;

  put(color, u, Color::gray());
  vis.discover_vertex(u, g);
  boost::tie(ei, ei_end) = out_edges(u, g);
  // Variable is needed to workaround a borland bug.
  TF &fn = static_cast<TF &>(func);
  if (fn(u, g)) {
    // If this vertex terminates the search, we push empty range
    stack.push_back(std::make_pair(u, std::make_pair(ei_end, ei_end)));
  } else {
    stack.push_back(std::make_pair(u, std::make_pair(ei, ei_end)));
  }
  while (!stack.empty()) {
    VertexInfo &back = stack.back();
    u = back.first;
    boost::tie(ei, ei_end) = back.second;
    stack.pop_back();
    while (ei != ei_end) {
      Vertex v = target(*ei, g);
      vis.examine_edge(*ei, g);
      ColorValue v_color = get(color, v);
      if (v_color == Color::white()) {
        vis.tree_edge(*ei, g);
        stack.push_back(std::make_pair(u, std::make_pair(++ei, ei_end)));
        u = v;
        put(color, u, Color::gray());
        vis.discover_vertex(u, g);
        boost::tie(ei, ei_end) = out_edges(u, g);
        if (fn(u, g)) {
          ei = ei_end;
        }
      } else if (v_color == Color::gray()) {
        vis.back_edge(*ei, g);
        ++ei;
      } else {
        vis.forward_or_cross_edge(*ei, g);
        ++ei;
      }
    }
    // put(color, u, Color::black());
    put(color, u, Color::white());
    vis.finish_vertex(u, g);
  }
}

template <class VertexListGraph, class DFSVisitor, class ColorMap>
void all_simple_paths(const VertexListGraph &g, DFSVisitor vis, ColorMap color,
                      typename boost::graph_traits<
                          VertexListGraph>::vertex_descriptor start_vertex) {
  typedef typename boost::graph_traits<VertexListGraph>::vertex_descriptor
  Vertex;
  BOOST_CONCEPT_ASSERT((boost::DFSVisitorConcept<DFSVisitor, VertexListGraph>));
  typedef typename boost::property_traits<ColorMap>::value_type ColorValue;
  typedef boost::color_traits<ColorValue> Color;

  // Initialize everything to white (unvisited)
  typename boost::graph_traits<VertexListGraph>::vertex_iterator ui, ui_end;
  for (boost::tie(ui, ui_end) = vertices(g); ui != ui_end; ++ui) {
    Vertex u = boost::implicit_cast<Vertex>(*ui);
    put(color, u, Color::white());
    vis.initialize_vertex(u, g);
  }

  vis.start_vertex(start_vertex, g);
  all_simple_paths_impl(g, start_vertex, vis, color,
                        boost::detail::nontruth2());
}

// Names parameter variant
template <class VertexListGraph, class P, class T, class R>
void all_simple_paths(const VertexListGraph &g,
                      const boost::bgl_named_params<P, T, R> &params) {
  typedef typename boost::graph_traits<VertexListGraph>::vertex_iterator vi;
  std::pair<vi, vi> verts = vertices(g);
  if (verts.first == verts.second)
    return;
  using namespace boost::graph::keywords;
  typedef boost::bgl_named_params<P, T, R> params_type;
  BOOST_GRAPH_DECLARE_CONVERTED_PARAMETERS(params_type, params);
  all_simple_paths(
      g, arg_pack[_visitor | make_dfs_visitor(boost::null_visitor())],
      boost::detail::make_color_map_from_arg_pack(g, arg_pack),
      static_cast<
          typename boost::graph_traits<VertexListGraph>::vertex_descriptor>(
          arg_pack[_root_vertex | *vertices(g).first]));
}

template <class VertexListGraph, class DFSVisitor>
void all_simple_paths(const VertexListGraph &g, DFSVisitor vis,
                      typename boost::graph_traits<
                          VertexListGraph>::vertex_descriptor start_vertex) {
  all_simple_paths(g, boost::visitor(vis).root_vertex(start_vertex));
}

template <class VertexListGraph, class DFSVisitor, class ColorMap>
void depth_first_search_reachable(
    const VertexListGraph &g, DFSVisitor vis, ColorMap color,
    typename boost::graph_traits<VertexListGraph>::vertex_descriptor
        start_vertex) {
  typedef typename boost::graph_traits<VertexListGraph>::vertex_descriptor
  Vertex;
  BOOST_CONCEPT_ASSERT((boost::DFSVisitorConcept<DFSVisitor, VertexListGraph>));
  typedef typename boost::property_traits<ColorMap>::value_type ColorValue;
  typedef boost::color_traits<ColorValue> Color;

  // Initialize everything to white (unvisited)
  typename boost::graph_traits<VertexListGraph>::vertex_iterator ui, ui_end;
  for (boost::tie(ui, ui_end) = vertices(g); ui != ui_end; ++ui) {
    Vertex u = boost::implicit_cast<Vertex>(*ui);
    put(color, u, Color::white());
    vis.initialize_vertex(u, g);
  }

  vis.start_vertex(start_vertex, g);
  boost::detail::depth_first_visit_impl(g, start_vertex, vis, color,
                                        boost::detail::nontruth2());
}

// Names parameter variant
template <class VertexListGraph, class P, class T, class R>
void
depth_first_search_reachable(const VertexListGraph &g,
                             const boost::bgl_named_params<P, T, R> &params) {
  typedef typename boost::graph_traits<VertexListGraph>::vertex_iterator vi;
  std::pair<vi, vi> verts = vertices(g);
  if (verts.first == verts.second)
    return;
  using namespace boost::graph::keywords;
  typedef boost::bgl_named_params<P, T, R> params_type;
  BOOST_GRAPH_DECLARE_CONVERTED_PARAMETERS(params_type, params);
  depth_first_search_reachable(
      g, arg_pack[_visitor | make_dfs_visitor(boost::null_visitor())],
      boost::detail::make_color_map_from_arg_pack(g, arg_pack),
      static_cast<
          typename boost::graph_traits<VertexListGraph>::vertex_descriptor>(
          arg_pack[_root_vertex | *vertices(g).first]));
}

template <class VertexListGraph, class DFSVisitor>
void depth_first_search(const VertexListGraph &g, DFSVisitor vis,
                        typename boost::graph_traits<
                            VertexListGraph>::vertex_descriptor start_vertex) {
  depth_first_search_reachable(g,
                               boost::visitor(vis).root_vertex(start_vertex));
}
}

// NOTE: Keep this function and the one below it in sync
void graph::run_traversal(Graph_t &g, Vertex_t source, Vertex_t sink,
                          std::vector<std::vector<std::string> > &paths,
                          Traversal traversal, util::Timer &timer) {
  targeted_path_visitor vis(sink, paths);
  switch (traversal) {
  case Traversal::depth_first_search:
    timer.start();
    ::depth_first_search(g, vis, source);
    timer.stop();
    break;
  case Traversal::all_simple_paths:
    timer.start();
    ::all_simple_paths(g, vis, source);
    timer.stop();
    break;
  case Traversal::all_edge_disjoint_paths:
    timer.start();
    ::all_edge_disjoint_paths(g, vis, source);
    timer.stop();
    break;
  }
}

// NOTE: Keep this function and the one above it in sync
void graph::run_traversal(Graph_t &g, Vertex_t source, Vertex_t sink,
                          std::vector<std::vector<std::string> > &paths,
                          Traversal traversal) {
  targeted_path_visitor vis(sink, paths);
  switch (traversal) {
  case Traversal::depth_first_search:
    ::depth_first_search(g, vis, source);
    break;
  case Traversal::all_simple_paths:
    ::all_simple_paths(g, vis, source);
    break;
  case Traversal::all_edge_disjoint_paths:
    ::all_edge_disjoint_paths(g, vis, source);
    break;
  }
}

std::ostream &graph::operator<<(std::ostream &out, const Traversal traversal) {
  switch (traversal) {
    case Traversal::depth_first_search:
      out << "Depth First Search";
      break;
    case Traversal::all_simple_paths:
      out << "All Simple Paths";
      break;
    case Traversal::all_edge_disjoint_paths:
      out << "All edge-disjoint paths";
      break;
  }
  return out;
}
