const actions = (() => {
  const FETCH = 'FETCH';
  const RECEIVE = 'RECEIVE';
  const TOGGLE_IS_PLAYING = 'TOGGLE_IS_PLAYING';
  const SET_NTH_FLOORPLAN = 'SET_NTH_FLOORPLAN';
  const NEXT = 'NEXT';

  return {
    FETCH,
    RECEIVE,
    TOGGLE_IS_PLAYING,
    SET_NTH_FLOORPLAN,
    NEXT,
  };
})();

const actionCreators = (() => {
  const {
    FETCH,
    RECEIVE,
    TOGGLE_IS_PLAYING,
    SET_NTH_FLOORPLAN,
    NEXT,
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

  function fastBackward() {
    return setNthFloorplan(-1);
  }

  function fastForward() {
    return setNthFloorplan(-2);
  }

  function next(numMacros) {
    return {
      type: NEXT,
      numMacros,
    };
  }

  return {
    fetch,
    receive,
    toggleIsPlaying,
    fastBackward,
    fastForward,
    next,
  };
})();

