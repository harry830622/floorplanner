const actions = (() => {
  const FETCH_DRAWING = 'FETCH_DRAWING';
  const RECEIVE_DRAWING = 'RECEIVE_DRAWING';
  const TOGGLE_IS_PLAYING = 'TOGGLE_IS_PLAYING';
  const SET_NTH_FLOORPLAN = 'SET_NTH_FLOORPLAN';
  const NEXT = 'NEXT';
  const SET_CONFIG = 'SET_CONFIG';
  const BACKWARD = 'BACKWARD';
  const FORWARD = 'FORWARD';

  return {
    FETCH_DRAWING,
    RECEIVE_DRAWING,
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
    FETCH_DRAWING,
    RECEIVE_DRAWING,
    TOGGLE_IS_PLAYING,
    SET_NTH_FLOORPLAN,
    NEXT,
    SET_CONFIG,
    BACKWARD,
    FORWARD,
  } = actions;

  function fetchDrawing() {
    return {
      type: FETCH_DRAWING,
    };
  }

  function receiveDrawing(drawing) {
    return {
      type: RECEIVE_DRAWING,
      drawing,
    };
  }

  function fetchDrawingAsync(benchmark) {
    return (dispatch) => {
      dispatch(fetchDrawing());

      return fetch('/drawing', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(benchmark),
      })
        .then(res => res.json())
        .then(json => dispatch(receiveDrawing(json)))
        .catch(err => Promise.reject(err));
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
    fetchDrawing,
    fetchDrawingAsync,
    receiveDrawing,
    toggleIsPlaying,
    setConfig,
    backward,
    forward,
    fastBackward,
    fastForward,
    next,
  };
})();

