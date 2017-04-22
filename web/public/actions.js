const actions = (() => {
  const TOGGLE_IS_PLAYING = 'TOGGLE_IS_PLAYING';
  const SET_SPEED = 'SET_SPEED';
  const FETCH = 'FETCH';
  const RECEIVE = 'RECEIVE';

  return {
    FETCH,
    RECEIVE,
    TOGGLE_IS_PLAYING,
    SET_SPEED,
  };
})();

const actionCreators = (() => {
  const { FETCH, RECEIVE, TOGGLE_IS_PLAYING, SET_SPEED } = actions;

  function fetch() {
    return {
      type: FETCH,
    };
  }

  function receive(payload) {
    return {
      type: RECEIVE,
      payload,
    };
  }

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
    fetch,
    receive,
    toggleIsPlaying,
    setSpeed,
  };
})();

