#include "../../Definitions.h"
#include "../../stencils/StencilFunctions.h"

#include "FGHStencil.h"
#include "FlowField.h"

namespace nseof {

namespace flowmodels {

namespace algebraic {

FGHStencil::FGHStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters) {}

void FGHStencil::apply(FlowField& flowField, int i, int j) {
  // Load local vortex viscostiy and velocities into the center layer of the
  // local array

  flowField.loadLocalNu2D(_localNu, i, j);
  loadLocalVelocity2D(flowField, _localVelocity, i, j);
  loadLocalMeshsize2D(_parameters, _localMeshsize, i, j);

  FLOAT* const values = flowField.getFGH().getVector(i, j);

  // Now the localVelocity array should contain lexicographically ordered
  // elements around the given index

  values[0] = computeF2DT(_localVelocity, _localMeshsize, _localNu, _parameters,
                          _parameters.timestep.dt);
  values[1] = computeG2DT(_localVelocity, _localMeshsize, _localNu, _parameters,
                          _parameters.timestep.dt);
}

void FGHStencil::apply(FlowField& flowField, int i, int j, int k) {
  // The same as in 2D, with slight modifications

  const int obstacle = flowField.getFlags().getValue(i, j, k);

  FLOAT* const values = flowField.getFGH().getVector(i, j, k);

  if ((obstacle & OBSTACLE_SELF) == 0) {  // If the cell is fluid
    flowField.loadLocalNu3D(_localNu, i, j, k);
    loadLocalVelocity3D(flowField, _localVelocity, i, j, k);
    loadLocalMeshsize3D(_parameters, _localMeshsize, i, j, k);

    if ((obstacle & OBSTACLE_RIGHT) == 0) {  // If the right cell is fluid
      values[0] = computeF3DT(_localVelocity, _localMeshsize, _localNu,
                              _parameters, _parameters.timestep.dt);
    }
    if ((obstacle & OBSTACLE_TOP) == 0) {
      values[1] = computeG3DT(_localVelocity, _localMeshsize, _localNu,
                              _parameters, _parameters.timestep.dt);
    }
    if ((obstacle & OBSTACLE_BACK) == 0) {
      values[2] = computeH3DT(_localVelocity, _localMeshsize, _localNu,
                              _parameters, _parameters.timestep.dt);
    }
  }
}
}
}
}