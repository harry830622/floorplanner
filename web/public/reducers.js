const reducer = (() => {
  const {
    FETCH_DRAWING,
    RECEIVE_DRAWING,
    TOGGLE_IS_PLAYING,
    SET_CONFIG,
    NEXT,
    BACKWARD,
    FORWARD,
    STEP_BACKWARD,
    STEP_FORWARD,
    FAST_BACKWARD,
    FAST_FORWARD,
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
    state = { nthIteration: -2, nthFloorplan: -1, nthMacro: -1 },
    action,
    rootState
  ) {
    const { drawing } = rootState;

    switch (action.type) {
      case NEXT: {
        const { iterations } = drawing;
        const numIterations = iterations.length;
        const numFloorplans = iterations[0].floorplans.length;
        const numMacros = iterations[0].floorplans[0].macros.length;
        const { nthIteration, nthFloorplan, nthMacro } = state;
        const newNthMacro = (nthMacro + 1) % numMacros;
        const newNthFloorplan = newNthMacro === 0
          ? (nthFloorplan + 1) % numFloorplans
          : nthFloorplan;
        const newNthIteration = newNthMacro === 0 && newNthFloorplan === 0
          ? nthIteration + 1
          : nthIteration;
        if (newNthIteration <= numIterations) {
          return Object.assign({}, state, {
            nthIteration: newNthIteration,
            nthFloorplan: newNthFloorplan,
            nthMacro: newNthMacro,
          });
        }
        return state;
      }
      case STEP_BACKWARD: {
        const newNthIteration = state.nthIteration - 1;
        if (newNthIteration >= 0) {
          return Object.assign({}, state, {
            nthIteration: newNthIteration,
            nthFloorplan: 0,
            nthMacro: 0,
          });
        }
        return state;
      }
      case STEP_FORWARD: {
        const newNthIteration = state.nthIteration + 1;
        const { iterations } = drawing;
        const numIterations = iterations.length;
        if (newNthIteration < numIterations) {
          return Object.assign({}, state, {
            nthIteration: newNthIteration,
            nthFloorplan: 0,
            nthMacro: 0,
          });
        }
        return state;
      }
      case FAST_BACKWARD: {
        return Object.assign({}, state, {
          nthIteration: -1,
          nthFloorplan: -1,
          nthMacro: -1,
        });
      }
      case FAST_FORWARD: {
        return Object.assign({}, state, {
          nthIteration: -2,
          nthFloorplan: -1,
          nthMacro: -1,
        });
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

