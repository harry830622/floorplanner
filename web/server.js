const Koa = require('koa');
const logger = require('koa-logger');
const serve = require('koa-static');
const send = require('koa-send');
const route = require('koa-route');
const bodyParser = require('koa-bodyparser');

const uuid = require('uuid');
const fs = require('fs-extra');

const exec = require('child_process').exec;

const app = new Koa();

app.use(logger());

app.use(bodyParser());

app.use(serve('./public'));

app.use(
  route.post('/drawing', async (ctx) => {
    const {
      benchmark: { name, blockInput, netInput },
      config: { alpha, sa, isDrawingAll },
    } = ctx.request.body;
    const id = uuid();
    const tmpDirName = id;
    const blockInputName = `${tmpDirName}/${name}.block`;
    const netInputName = `${tmpDirName}/${name}.net`;
    const outputName = `${tmpDirName}/${name}.out`;
    const drawingName = `${tmpDirName}/${name}.json`;

    await new Promise((resolve, reject) => {
      fs.mkdir(tmpDirName, (err) => {
        if (err) {
          reject(err);
        }
        resolve();
      });
    });

    await new Promise((resolve, reject) => {
      fs.writeFile(blockInputName, blockInput, (err) => {
        if (err) {
          reject(err);
        }
        resolve();
      });
    });

    await new Promise((resolve, reject) => {
      fs.writeFile(netInputName, netInput, (err) => {
        if (err) {
          reject(err);
        }
        resolve();
      });
    });

    await new Promise((resolve, reject) => {
      exec(
        `./fp_static --sa ${sa} ${isDrawingAll ? '--draw-all' : '--draw-only-best'} ${drawingName} ${alpha} ${blockInputName} ${netInputName} ${outputName}`,
        (err) => {
          if (err) {
            reject(err);
          }
          resolve();
        }
      );
    });

    await new Promise((resolve, reject) => {
      fs.readFile(drawingName, { encoding: 'utf-8' }, (err, data) => {
        if (err) {
          reject(err);
        }
        resolve(JSON.parse(data));
      });
    }).then(
      drawing =>
        new Promise((resolve, reject) => {
          fs.writeFile(drawingName, JSON.stringify(drawing), (err) => {
            if (err) {
              reject(err);
            }
            resolve();
          });
        })
    );

    await send(ctx, drawingName);

    await new Promise((resolve, reject) => {
      fs.remove(tmpDirName, (err) => {
        if (err) {
          reject(err);
        }
        resolve();
      });
    });
  })
);

app.listen(3000); // Change to 80 when in production.

