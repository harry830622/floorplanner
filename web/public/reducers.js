const reducers = (() => {
  const { combineReducers } = Redux;

  const { TOGGLE_IS_PLAYING, SET_SPEED } = actions;

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

  return combineReducers({
    isPlaying,
    speed,
  });
})();

