const {
  fetchDrawing,
  fetchDrawingAsync,
  receiveDrawing,
  toggleIsPlaying,
  setConfig,
  next,
  backward,
  forward,
  stepBackward,
  stepForward,
  fastBackward,
  fastForward,
} = actionCreators;

const openInput = document.querySelector('#open-input');
const openLink = document.querySelector('#open-link');

openInput.addEventListener(
  'change',
  (e) => {
    const files = e.target.files;

    if (files.length === 2) {
      let blockText = '';
      let netText = '';
      let numLoadedFiles = 0;
      for (let i = 0; i < files.length; i += 1) {
        const file = files[i];
        const fileReader = new FileReader();
        fileReader.onload = (e) => {
          numLoadedFiles += 1;
          const text = e.target.result;
          if (text[0] === 'O') {
            blockText = text;
          } else {
            netText = text;
          }
          if (numLoadedFiles === 2) {
            const { config } = store.getState();
            store.dispatch(
              fetchDrawingAsync({
                benchmark: {
                  name: file.name.split('.')[0],
                  blockInput: blockText,
                  netInput: netText,
                },
                config,
              })
            );
          }
        };
        fileReader.readAsText(file);
      }
    }
  },
  false
);

openLink.addEventListener(
  'click',
  () => {
    openInput.click();
  },
  false
);

const uploadInput = document.querySelector('#upload-input');
const uploadLink = document.querySelector('#upload-link');

uploadInput.addEventListener(
  'change',
  (e) => {
    const file = e.target.files[0];

    if (file) {
      store.dispatch(fetchDrawing());

      const fileReader = new FileReader();
      fileReader.onload = (e) => {
        const drawing = JSON.parse(e.target.result);
        store.dispatch(receiveDrawing(drawing));
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

store.subscribe(() => {
  const { isFetchingDrawing, frame } = store.getState();

  if (isFetchingDrawing && frame.nthIteration !== -2) {
    store.dispatch(fastForward());
  }
});

const loadingDimmer = document.querySelector('#loading-dimmer');

store.subscribe(() => {
  const { isFetchingDrawing } = store.getState();

  if (isFetchingDrawing) {
    loadingDimmer.classList.add('active');
  } else {
    loadingDimmer.classList.remove('active');
  }
});

const settingsLink = document.querySelector('#settings-link');

settingsLink.addEventListener(
  'click',
  () => {
    $('#settings-modal').modal('show');
  },
  false
);

const alphaInput = document.querySelector('#alpha-input');

alphaInput.addEventListener(
  'change',
  () => {
    store.dispatch(setConfig({ alpha: alphaInput.value }));
  },
  false
);

const saSelect = document.querySelector('#sa-select');

saSelect.addEventListener(
  'change',
  () => {
    store.dispatch(setConfig({ sa: saSelect.value }));
  },
  false
);

const isDrawingAllInput = document.querySelector('#is-drawing-all-input');

isDrawingAllInput.addEventListener(
  'change',
  () => {
    store.dispatch(setConfig({ isDrawingAll: isDrawingAllInput.checked }));
  },
  false
);

const playBtn = document.querySelector('#play-btn');
const backwardBtn = document.querySelector('#backward-btn');
const forwardBtn = document.querySelector('#forward-btn');
const stepBackwardBtn = document.querySelector('#step-backward-btn');
const stepForwardBtn = document.querySelector('#step-forward-btn');
const fastBackwardBtn = document.querySelector('#fast-backward-btn');
const fastForwardBtn = document.querySelector('#fast-forward-btn');

playBtn.addEventListener(
  'click',
  () => {
    const { isFetchingDrawing, drawing } = store.getState();

    if (drawing.iterations && !isFetchingDrawing) {
      store.dispatch(toggleIsPlaying());
    }
  },
  false
);

backwardBtn.addEventListener(
  'click',
  () => {
    const { isFetchingDrawing, isPlaying, drawing } = store.getState();

    if (drawing.iterations && !isFetchingDrawing) {
      store.dispatch(backward());
    }
  },
  false
);

forwardBtn.addEventListener(
  'click',
  () => {
    const { isFetchingDrawing, isPlaying, drawing } = store.getState();

    if (drawing.iterations && !isFetchingDrawing) {
      store.dispatch(forward());
    }
  },
  false
);

stepBackwardBtn.addEventListener(
  'click',
  () => {
    const { isFetchingDrawing, isPlaying, drawing } = store.getState();

    if (drawing.iterations && !isFetchingDrawing) {
      if (isPlaying) {
        store.dispatch(toggleIsPlaying());
      }
      store.dispatch(stepBackward());
    }
  },
  false
);

stepForwardBtn.addEventListener(
  'click',
  () => {
    const { isFetchingDrawing, isPlaying, drawing } = store.getState();

    if (drawing.iterations && !isFetchingDrawing) {
      if (isPlaying) {
        store.dispatch(toggleIsPlaying());
      }
      store.dispatch(stepForward());
    }
  },
  false
);

fastBackwardBtn.addEventListener(
  'click',
  () => {
    const { isFetchingDrawing, isPlaying, drawing } = store.getState();

    if (drawing.iterations && !isFetchingDrawing) {
      fastBackwardBtn.classList.add('disabled');
      stepBackwardBtn.classList.remove('disabled');
      backwardBtn.classList.remove('disabled');
      playBtn.classList.remove('disabled');
      forwardBtn.classList.remove('disabled');
      stepForwardBtn.classList.remove('disabled');
      fastForwardBtn.classList.remove('disabled');

      if (isPlaying) {
        store.dispatch(toggleIsPlaying());
      }

      store.dispatch(fastBackward());
    }
  },
  false
);

fastForwardBtn.addEventListener(
  'click',
  () => {
    const { isFetchingDrawing, isPlaying, drawing } = store.getState();

    if (drawing.iterations && !isFetchingDrawing) {
      fastBackwardBtn.classList.remove('disabled');
      stepBackwardBtn.classList.add('disabled');
      backwardBtn.classList.add('disabled');
      playBtn.classList.add('disabled');
      forwardBtn.classList.add('disabled');
      stepForwardBtn.classList.add('disabled');
      fastForwardBtn.classList.add('disabled');

      if (isPlaying) {
        store.dispatch(toggleIsPlaying());
      }

      store.dispatch(fastForward());
    }
  },
  false
);

store.subscribe(() => {
  const { drawing } = store.getState();

  if (drawing.iterations) {
    fastBackwardBtn.classList.remove('disabled');
  } else {
    fastBackwardBtn.classList.add('disabled');
    stepBackwardBtn.classList.add('disabled');
    backwardBtn.classList.add('disabled');
    playBtn.classList.add('disabled');
    forwardBtn.classList.add('disabled');
    stepForwardBtn.classList.add('disabled');
    fastForwardBtn.classList.add('disabled');
  }
});

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

store.subscribe(() => {
  const { config } = store.getState();

  if (config.speed === 1) {
    backwardBtn.style.display = 'none';
    stepBackwardBtn.style.display = 'inline-block';
  } else {
    backwardBtn.style.display = 'inline-block';
    stepBackwardBtn.style.display = 'none';
  }

  if (config.speed === 3) {
    forwardBtn.style.display = 'none';
    stepForwardBtn.style.display = 'inline-block';
  } else {
    forwardBtn.style.display = 'inline-block';
    stepForwardBtn.style.display = 'none';
  }
});

store.subscribe(() => {
  const { frame, drawing } = store.getState();

  if (drawing.iterations) {
    const numIterations = drawing.iterations.length;

    if (frame.nthIteration <= 0) {
      stepBackwardBtn.classList.add('disabled');
    } else {
      stepBackwardBtn.classList.remove('disabled');
    }

    if (frame.nthIteration >= numIterations - 1) {
      stepForwardBtn.classList.add('disabled');
    } else {
      stepForwardBtn.classList.remove('disabled');
    }
  }
});

const nameStatValue = document.querySelector('#name-stat>div.value');
const outlineWidthStatValue = document.querySelector(
  '#outline-width-stat>div.value'
);
const outlineHeightStatValue = document.querySelector(
  '#outline-height-stat>div.value'
);
const alphaStatValue = document.querySelector('#alpha-stat>div.value');
const widthStatValue = document.querySelector('#width-stat>div.value');
const heightStatValue = document.querySelector('#height-stat>div.value');
const areaStatValue = document.querySelector('#area-stat>div.value');
const wirelengthStatValue = document.querySelector(
  '#wirelength-stat>div.value'
);
const costStatValue = document.querySelector('#cost-stat>div.value');
const nthIterationStatValue = document.querySelector(
  '#nth-iteration-stat>div.value'
);
const temperatureStatValue = document.querySelector(
  '#temperature-stat>div.value'
);
const nthPerturbationStatValue = document.querySelector(
  '#nth-perturbation-stat>div.value'
);

store.subscribe(() => {
  const { drawing, frame } = store.getState();
  const {
    benchmark,
    alpha,
    outline,
    best_floorplan: bestFloorplan,
    initial_floorplan: initialFloorplan,
    iterations,
  } = drawing;
  const { nthIteration, nthFloorplan } = frame;

  if (benchmark) {
    nameStatValue.innerHTML = benchmark;
    outlineWidthStatValue.innerHTML = outline.width;
    outlineHeightStatValue.innerHTML = outline.height;
    alphaStatValue.innerHTML = alpha;
  }

  if (nthIteration === -2) {
    if (bestFloorplan) {
      widthStatValue.innerHTML = bestFloorplan.width;
      heightStatValue.innerHTML = bestFloorplan.height;
      areaStatValue.innerHTML = bestFloorplan.area;
      wirelengthStatValue.innerHTML = bestFloorplan.wirelength;
      costStatValue.innerHTML =
        alpha * bestFloorplan.area + (1 - alpha) * bestFloorplan.wirelength;
    }
  } else if (nthIteration === -1) {
    if (initialFloorplan) {
      widthStatValue.innerHTML = initialFloorplan.width;
      heightStatValue.innerHTML = initialFloorplan.height;
      areaStatValue.innerHTML = initialFloorplan.area;
      wirelengthStatValue.innerHTML = initialFloorplan.wirelength;
      costStatValue.innerHTML =
        alpha * initialFloorplan.area +
        (1 - alpha) * initialFloorplan.wirelength;
    }
  } else if (iterations) {
    const numIterations = iterations.length;
    const numFloorplans = iterations[0].floorplans.length;
    const iteration = iterations[nthIteration];

    if (iteration) {
      temperatureStatValue.innerHTML = iteration.temperature;
      nthIterationStatValue.innerHTML = `${nthIteration + 1}/${numIterations}`;
      nthPerturbationStatValue.innerHTML = `${nthFloorplan + 1}/${numFloorplans}`;

      const floorplan = iteration.floorplans[nthFloorplan];
      if (floorplan) {
        widthStatValue.innerHTML = floorplan.width;
        heightStatValue.innerHTML = floorplan.height;
        areaStatValue.innerHTML = floorplan.area;
        wirelengthStatValue.innerHTML = floorplan.wirelength;
        costStatValue.innerHTML =
          alpha * floorplan.area + (1 - alpha) * floorplan.wirelength;
      }
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
  const { isFetchingDrawing, frame, drawing } = store.getState();
  const { nthIteration, nthFloorplan, nthMacro } = frame;
  const {
    outline,
    best_floorplan: bestFloorplan,
    initial_floorplan: initialFloorplan,
    iterations,
  } = drawing;

  if (isFetchingDrawing) {
    stage.removeChildren();
  }

  if (nthIteration === -2) {
    if (bestFloorplan) {
      stage.removeChildren();

      const scale =
        computeScale(
          { width: canvasWidth, height: canvasHeight },
          outline,
          bestFloorplan
        ) * 0.97;

      drawOutline(outline, scale);

      bestFloorplan.macros.forEach(({
        name,
        lower_left: lowerLeft,
        upper_right: upperRight,
      }) => {
        drawMacro({ name, lowerLeft, upperRight }, scale);
      });
    }
  } else if (nthIteration === -1) {
    if (initialFloorplan) {
      stage.removeChildren();

      const scale =
        computeScale(
          { width: canvasWidth, height: canvasHeight },
          outline,
          initialFloorplan
        ) * 0.97;

      drawOutline(outline, scale);

      initialFloorplan.macros.forEach(({
        name,
        lower_left: lowerLeft,
        upper_right: upperRight,
      }) => {
        drawMacro({ name, lowerLeft, upperRight }, scale);
      });
    }
  } else if (iterations) {
    const iteration = iterations[nthIteration];
    if (iteration) {
      stage.removeChildren();

      const floorplan = iteration.floorplans[nthFloorplan];
      if (floorplan) {
        const scale =
          computeScale(
            { width: canvasWidth, height: canvasHeight },
            outline,
            floorplan
          ) * 0.97;

        drawOutline(outline, scale);

        floorplan.macros.slice(0, nthMacro + 1).forEach(({
          name,
          lower_left: lowerLeft,
          upper_right: upperRight,
        }) => {
          drawMacro({ name, lowerLeft, upperRight }, scale);
        });
      }
    }
  }
});

let t = 0;
function animate() {
  renderer.render(stage);

  const { isPlaying, config, frame, drawing } = store.getState();

  if (drawing.iterations) {
    if (isPlaying) {
      const numIterations = drawing.iterations.length;

      if (frame.nthIteration >= numIterations) {
        store.dispatch(toggleIsPlaying());
      }

      const { speed } = config;

      t += 1;
      switch (speed) {
        case 1: {
          if (t % 60 === 0) {
            store.dispatch(next());
          }
          break;
        }
        case 2: {
          if (t % 20 === 0) {
            store.dispatch(next());
          }
          break;
        }
        case 3: {
          if (t % 5 === 0) {
            store.dispatch(next());
          }
          break;
        }
        default:
          break;
      }
    }
  }

  requestAnimationFrame(animate);
}

animate();

