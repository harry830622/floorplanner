const reducer = (() => {
  const {
    FETCH_DRAWING,
    RECEIVE_DRAWING,
    TOGGLE_IS_PLAYING,
    SET_NTH_FLOORPLAN,
    NEXT,
    SET_CONFIG,
    BACKWARD,
    FORWARD,
  } = actions;

  function drawing(state = {}, action) {
    switch (action.type) {
      case FETCH_DRAWING: {
        return {};
      }
      case RECEIVE_DRAWING: {
        return action.drawing;
      }
      default: {
        return state;
      }
    }
  }

  function isFetchingDrawing(state = false, action) {
    switch (action.type) {
      case FETCH_DRAWING: {
        return true;
      }
      case RECEIVE_DRAWING: {
        return false;
      }
      default: {
        return state;
      }
    }
  }

  function isPlaying(state = false, action) {
    switch (action.type) {
      case TOGGLE_IS_PLAYING: {
        return !state;
      }
      default: {
        return state;
      }
    }
  }

  function frame(
    state = { nthFloorplan: -2, nthMacro: -1 },
    action,
    rootState
  ) {
    const { drawing, config } = rootState;

    switch (action.type) {
      case SET_NTH_FLOORPLAN: {
        if (action.nthFloorplan < 0) {
          return Object.assign({}, state, {
            nthFloorplan: action.nthFloorplan,
            nthMacro: -1,
          });
        }
        if (drawing.iterations) {
          return Object.assign({}, state, {
            nthFloorplan: action.nthFloorplan,
            nthMacro: -1,
          });
        }
        return state;
      }
      case NEXT: {
        if (drawing.iterations) {
          const { iterations } = drawing;
          const numMacros = iterations[0].floorplans[0].macros.length;
          const newNthMacro = (state.nthMacro + 1) % numMacros;
          const newNthFloorplan = newNthMacro === 0
            ? state.nthFloorplan + 1
            : state.nthFloorplan;
          return Object.assign({}, state, {
            nthFloorplan: newNthFloorplan,
            nthMacro: newNthMacro,
          });
        }
        return state;
      }
      case BACKWARD: {
        if (drawing.iterations) {
          if (config.speed === 1) {
            const { iterations } = drawing;
            const numFloorplans = iterations[0].floorplans.length;
            const newNthFloorplan =
              (Math.floor(state.nthFloorplan / numFloorplans) - 1) *
              numFloorplans;
            return Object.assign({}, state, {
              nthFloorplan: Math.max(0, newNthFloorplan),
              nthMacro: -1,
            });
          }
        }
        return state;
      }
      case FORWARD: {
        if (drawing.iterations) {
          if (config.speed === 3) {
            const { iterations } = drawing;
            const numIterations = iterations.length;
            const numFloorplans = iterations[0].floorplans.length;
            const newNthFloorplan =
              (Math.floor(state.nthFloorplan / numFloorplans) + 1) *
              numFloorplans;
            return Object.assign({}, state, {
              nthFloorplan: Math.min(
                numIterations * numFloorplans - 2,
                newNthFloorplan
              ),
              nthMacro: -1,
            });
          }
        }
        return state;
      }
      default: {
        return state;
      }
    }
  }

  function config(
    state = { alpha: 0.5, sa: 'classical', isDrawingAll: false, speed: 1 },
    action
  ) {
    switch (action.type) {
      case SET_CONFIG: {
        return Object.assign({}, state, action.config);
      }
      case BACKWARD: {
        if (state.speed > 1) {
          return Object.assign({}, state, { speed: state.speed - 1 });
        }
        return state;
      }
      case FORWARD: {
        if (state.speed < 3) {
          return Object.assign({}, state, { speed: state.speed + 1 });
        }
        return state;
      }
      default: {
        return state;
      }
    }
  }

  return (state = {}, action) => ({
    drawing: drawing(state.drawing, action),
    isFetchingDrawing: isFetchingDrawing(state.isFetchingDrawing, action),
    isPlaying: isPlaying(state.isPlaying, action),
    frame: frame(state.frame, action, state),
    config: config(state.config, action),
  });
})();

