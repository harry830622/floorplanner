const actions = (() => {
  const FETCH_DRAWING = 'FETCH_DRAWING';
  const RECEIVE_DRAWING = 'RECEIVE_DRAWING';
  const TOGGLE_IS_PLAYING = 'TOGGLE_IS_PLAYING';
  const SET_CONFIG = 'SET_CONFIG';
  const NEXT = 'NEXT';
  const BACKWARD = 'BACKWARD';
  const FORWARD = 'FORWARD';
  const STEP_BACKWARD = 'STEP_BACKWARD';
  const STEP_FORWARD = 'STEP_FORWARD';
  const FAST_BACKWARD = 'FAST_BACKWARD';
  const FAST_FORWARD = 'FAST_FORWARD';

  return {
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
  };
})();

const actionCreators = (() => {
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

  function fetchDrawingAsync(payload) {
    return (dispatch) => {
      dispatch(fetchDrawing());

      return fetch('/drawing', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(payload),
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

  function setConfig(config) {
    return {
      type: SET_CONFIG,
      config,
    };
  }

  function next() {
    return {
      type: NEXT,
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

  function stepBackward() {
    return {
      type: STEP_BACKWARD,
    };
  }

  function stepForward() {
    return {
      type: STEP_FORWARD,
    };
  }

  function fastBackward() {
    return {
      type: FAST_BACKWARD,
    };
  }

  function fastForward() {
    return {
      type: FAST_FORWARD,
    };
  }

  return {
    fetchDrawing,
    fetchDrawingAsync,
    receiveDrawing,
    toggleIsPlaying,
    setConfig,
    backward,
    forward,
    stepBackward,
    stepForward,
    fastBackward,
    fastForward,
    next,
  };
})();

