const actions = (() => {
  const FETCH = 'FETCH';
  const RECEIVE = 'RECEIVE';
  const TOGGLE_IS_PLAYING = 'TOGGLE_IS_PLAYING';
  const SET_NTH_FLOORPLAN = 'SET_NTH_FLOORPLAN';
  const NEXT = 'NEXT';
  const SET_CONFIG = 'SET_CONFIG';
  const BACKWARD = 'BACKWARD';
  const FORWARD = 'FORWARD';

  return {
    FETCH,
    RECEIVE,
    TOGGLE_IS_PLAYING,
    SET_NTH_FLOORPLAN,
    NEXT,
    SET_CONFIG,
    BACKWARD,
    FORWARD,
  };
})();

const actionCreators = (() => {
  const {
    FETCH,
    RECEIVE,
    TOGGLE_IS_PLAYING,
    SET_NTH_FLOORPLAN,
    NEXT,
    SET_CONFIG,
    BACKWARD,
    FORWARD,
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

  function setNthFloorplan(nthFloorplan) {
    return {
      type: SET_NTH_FLOORPLAN,
      nthFloorplan,
    };
  }

  function next(numMacros) {
    return {
      type: NEXT,
      numMacros,
    };
  }

  function setConfig(config) {
    return {
      type: SET_CONFIG,
      config,
    };
  }

  function backward() {
    return {
      type: BACKWARD,
    };
  }

  function forward() {
    return {
      type: FORWARD,
    };
  }

  function fastBackward() {
    return setNthFloorplan(-1);
  }

  function fastForward() {
    return setNthFloorplan(-2);
  }

  return {
    fetch,
    receive,
    toggleIsPlaying,
    backward,
    forward,
    fastBackward,
    fastForward,
    next,
  };
})();

