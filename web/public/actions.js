const actions = (() => {
  const FETCH = 'FETCH';
  const RECEIVE = 'RECEIVE';
  const TOGGLE_IS_PLAYING = 'TOGGLE_IS_PLAYING';
  const SET_SPEED = 'SET_SPEED';
  const SET_NTH_ITERATION = 'SET_NTH_ITERATION';

  return {
    FETCH,
    RECEIVE,
    TOGGLE_IS_PLAYING,
    SET_SPEED,
    SET_NTH_ITERATION,
  };
})();

const actionCreators = (() => {
  const {
    FETCH,
    RECEIVE,
    TOGGLE_IS_PLAYING,
    SET_SPEED,
    SET_NTH_ITERATION,
  } = actions;

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

  function setNthIteration(nthIteration) {
    return {
      type: SET_NTH_ITERATION,
      nthIteration,
    };
  }

  return {
    fetch,
    receive,
    toggleIsPlaying,
    setSpeed,
    setNthIteration,
  };
})();

