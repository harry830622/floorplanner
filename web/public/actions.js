const actions = (() => {
  const TOGGLE_IS_PLAYING = 'TOGGLE_IS_PLAYING';
  const SET_SPEED = 'SET_SPEED';

  return {
    TOGGLE_IS_PLAYING,
    SET_SPEED,
  };
})();

const actionCreators = (() => {
  const { TOGGLE_IS_PLAYING, SET_SPEED } = actions;

  function toggleIsPlaying() {
    return {
      type: TOGGLE_IS_PLAYING,
    };
  }

  function setSpeed(speed) {
    return {
      type: SET_SPEED,
      speed,
    };
  }

  return {
    toggleIsPlaying,
    setSpeed,
  };
})();

