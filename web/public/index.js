const { toggleIsPlaying, setSpeed } = actionCreators;

const canvasWidth = 700;
const canvasHeight = 700;

const renderer = PIXI.autoDetectRenderer({
  width: canvasWidth,
  height: canvasHeight,
  view: document.querySelector('#floorplan-canvas'),
  backgroundColor: 0xf9f7f7,
});

const stage = new PIXI.Container();

renderer.render(stage);

const playBtn = document.querySelector('#play-btn');

playBtn.addEventListener(
  'click',
  () => {
    store.dispatch(toggleIsPlaying());
  },
  false
);

store.subscribe(() => {
  const { isPlaying } = store.getState();

  if (isPlaying) {
    playBtn.querySelector('i.play').style.display = 'none';
    playBtn.querySelector('i.pause').style.display = 'inline-block';
  } else {
    playBtn.querySelector('i.pause').style.display = 'none';
    playBtn.querySelector('i.play').style.display = 'inline-block';
  }
});

