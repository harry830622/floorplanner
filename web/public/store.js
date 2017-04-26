const store = (() => {
  const { createStore, applyMiddleware } = Redux;
  const { default: thunk } = ReduxThunk;
  const { default: logger } = reduxLogger;

  return createStore(reducer, {}, applyMiddleware(thunk, logger));
})();

