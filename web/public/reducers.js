const reducer = (() => {
  const {
    FETCH,
    RECEIVE,
    TOGGLE_IS_PLAYING,
    SET_NTH_FLOORPLAN,
    NEXT,
  } = actions;

  function drawing(state = {}, action) {
    switch (action.type) {
      case FETCH: {
        return {};
      }
      case RECEIVE: {
        return action.payload;
      }
      default: {
        return state;
      }
    }
  }

  function isFetching(state = false, action) {
    switch (action.type) {
      case FETCH: {
        return true;
      }
      case RECEIVE: {
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

  function frame(state = { nthFloorplan: -2, nthMacro: -1 }, action, drawing) {
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
      default: {
        return state;
      }
    }
  }

  return (state = {}, action) => ({
    drawing: drawing(state.drawing, action),
    isFetching: isFetching(state.isFetching, action),
    isPlaying: isPlaying(state.isPlaying, action),
    frame: frame(state.frame, action, state.drawing),
  });
})();

