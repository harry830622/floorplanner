const {
  fetch,
  receive,
  toggleIsPlaying,
  setSpeed,
  setNthIteration,
} = actionCreators;

const uploadInput = document.querySelector('#upload-input');
const uploadLink = document.querySelector('#upload-link');

uploadInput.addEventListener(
  'change',
  (e) => {
    const file = e.target.files[0];

    if (file) {
      store.dispatch(fetch());

      const fileReader = new FileReader();
      fileReader.onload = (e) => {
        const payload = JSON.parse(e.target.result);
        store.dispatch(receive(payload));
      };
      fileReader.readAsText(file);
    }
  },
  false
);

uploadLink.addEventListener(
  'click',
  () => {
    uploadInput.click();
  },
  false
);

const playBtn = document.querySelector('#play-btn');
const fastBackwardBtn = document.querySelector('#fast-backward-btn');
const fastForwardBtn = document.querySelector('#fast-forward-btn');

playBtn.addEventListener(
  'click',
  () => {
    const { isFetching } = store.getState();

    if (!isFetching) {
      store.dispatch(toggleIsPlaying());
    }
  },
  false
);

fastBackwardBtn.addEventListener(
  'click',
  () => {
    const { isFetching } = store.getState();

    if (!isFetching) {
      store.dispatch(setNthIteration(-1));
    }
  },
  false
);

fastForwardBtn.addEventListener(
  'click',
  () => {
    const { isFetching } = store.getState();

    if (!isFetching) {
      store.dispatch(setNthIteration(-2));
    }
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

const canvasWidth = 700;
const canvasHeight = 700;

const renderer = PIXI.autoDetectRenderer({
  width: canvasWidth,
  height: canvasHeight,
  view: document.querySelector('#floorplan-canvas'),
  backgroundColor: 0xf9f7f7,
});

const stage = new PIXI.Container();

function computeScale({ width: targetWidth, height: targetHeight }, ...objs) {
  const { width: maxObjWidth, height: maxObjHeight } = objs.reduce(
    (max, obj) => ({
      width: Math.max(max.width, obj.width),
      height: Math.max(max.height, obj.height),
    }),
    { width: 0, height: 0 }
  );

  return maxObjWidth > maxObjHeight
    ? targetWidth / maxObjWidth
    : targetHeight / maxObjHeight;
}

function createText({ x, y, text }, style) {
  const t = new PIXI.Text(text, style);

  t.x = x;
  t.y = y;

  return t;
}

function createLine({ x: fromX, y: fromY }, { x: toX, y: toY }, style) {
  const graphics = new PIXI.Graphics();

  graphics.lineStyle(1, style.color).moveTo(fromX, fromY).lineTo(toX, toY);

  return graphics;
}

function createRect({ x, y, width, height }, style) {
  const graphics = new PIXI.Graphics();

  graphics
    .lineStyle(1, 0x000000)
    .beginFill(style.color)
    .drawRect(x, y, width, height)
    .endFill();

  return graphics;
}

function drawOutline({ width, height }, scale) {
  stage.addChild(
    createLine(
      { x: 0, y: canvasHeight - height * scale },
      { x: width * scale, y: canvasHeight - height * scale },
      { width: 2, color: 0xff0000 }
    )
  );
  stage.addChild(
    createLine(
      { x: width * scale, y: canvasHeight - height * scale },
      { x: width * scale, y: canvasHeight },
      { width: 2, color: 0xff0000 }
    )
  );
}

function drawMacro({ name, lowerLeft, upperRight }, scale) {
  const x = lowerLeft.x * scale;
  const y = canvasHeight - upperRight.y * scale;
  const width = (upperRight.x - lowerLeft.x) * scale;
  const height = (upperRight.y - lowerLeft.y) * scale;

  stage.addChild(createRect({ x, y, width, height }, { color: 0x3f72af }));
  stage.addChild(
    createText(
      { x: x + 10 * scale, y: y + 10 * scale, text: name },
      { fontSize: 12 }
    )
  );
}

store.subscribe(() => {
  const { isFetching, nthIteration, drawing } = store.getState();

  if (isFetching) {
    stage.removeChildren();
  }

  if (nthIteration === -2 && drawing.best_floorplan) {
    stage.removeChildren();

    const { outline, best_floorplan } = drawing;
    const scale =
      computeScale(
        {
          width: canvasWidth,
          height: canvasHeight,
        },
        outline,
        best_floorplan
      ) * 0.97;
    drawOutline(outline, scale);
    best_floorplan.macros.forEach(({
      name,
      lower_left: lowerLeft,
      upper_right: upperRight,
    }) => {
      drawMacro({ name, lowerLeft, upperRight }, scale);
    });
  } else if (nthIteration === -1 && drawing.initial_floorplan) {
    stage.removeChildren();

    const { outline, initial_floorplan } = drawing;
    const scale =
      computeScale(
        {
          width: canvasWidth,
          height: canvasHeight,
        },
        outline,
        initial_floorplan
      ) * 0.97;
    drawOutline(outline, scale);
    initial_floorplan.macros.forEach(({
      name,
      lower_left: lowerLeft,
      upper_right: upperRight,
    }) => {
      drawMacro({ name, lowerLeft, upperRight }, scale);
    });
  }
});

function animate() {
  renderer.render(stage);

  requestAnimationFrame(animate);
}

animate();

