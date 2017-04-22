const { toggleIsPlaying, setSpeed, fetch, receive } = actionCreators;

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

store.subscribe(() => {
  const { isFetching, isPlaying } = store.getState();

  if (!isFetching) {
    if (isPlaying) {
      playBtn.querySelector('i.play').style.display = 'none';
      playBtn.querySelector('i.pause').style.display = 'inline-block';
    } else {
      playBtn.querySelector('i.pause').style.display = 'none';
      playBtn.querySelector('i.play').style.display = 'inline-block';
    }
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

function computeScale(
  { width: objWidth, height: objHeight },
  { width: targetWidth, height: targetHeight }
) {
  return objWidth > objHeight
    ? targetWidth / objWidth
    : targetHeight / objHeight;
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

store.subscribe(() => {
  const { drawing } = store.getState();

  if (drawing.best_floorplan) {
    const { outline, best_floorplan } = drawing;
    const scale =
      computeScale(outline, {
        width: canvasWidth,
        height: canvasHeight,
      }) * 0.97;
    stage.addChild(
      createLine(
        { x: 0, y: canvasHeight - outline.height * scale },
        { x: outline.width * scale, y: canvasHeight - outline.height * scale },
        { width: 2, color: 0xff0000 }
      )
    );
    stage.addChild(
      createLine(
        { x: outline.width * scale, y: canvasHeight - outline.height * scale },
        { x: outline.width * scale, y: canvasHeight },
        { width: 2, color: 0xff0000 }
      )
    );
    best_floorplan.macros.forEach((macro) => {
      const { name, lower_left, upper_right } = macro;
      const rect_x = lower_left.x * scale;
      const rect_y = canvasHeight - upper_right.y * scale;
      const rect_width = (upper_right.x - lower_left.x) * scale;
      const rect_height = (upper_right.y - lower_left.y) * scale;

      stage.addChild(
        createRect(
          {
            x: rect_x,
            y: rect_y,
            width: rect_width,
            height: rect_height,
          },
          { color: 0x3f72af }
        )
      );
      stage.addChild(
        createText(
          {
            x: rect_x + 10 * scale,
            y: rect_y + 10 * scale,
            text: name,
          },
          { fontSize: 12 }
        )
      );
    });
  } else {
    stage.removeChildren();
  }
});

function animate() {
  renderer.render(stage);

  requestAnimationFrame(animate);
}

animate();

