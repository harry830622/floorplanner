const reducers = (() => {
  const { combineReducers } = Redux;

  const {
    FETCH,
    RECEIVE,
    TOGGLE_IS_PLAYING,
    SET_SPEED,
    SET_NTH_ITERATION,
  } = actions;

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

  function speed(state = 3, action) {
    switch (action.type) {
      case SET_SPEED: {
        return action.speed;
      }
      default: {
        return state;
      }
    }
  }

  function nthIteration(state = -2, action) {
    switch (action.type) {
      case SET_NTH_ITERATION: {
        return action.nthIteration;
      }
      default: {
        return state;
      }
    }
  }

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

  return combineReducers({
    isFetching,
    isPlaying,
    speed,
    nthIteration,
    drawing,
  });
})();

