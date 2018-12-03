import fs from 'fs-extra';
import path from 'path';
import { spawn } from 'promisify-child-process';
import {
  apply,
  compose,
  concat,
  cond,
  endsWith,
  equals,
  filter,
  fromPairs,
  map,
  replace,
  split,
  T,
  toLower,
  when,
} from 'ramda';
import { CXX, CXX_FLAGS, CXX_FINAL_FLAGS } from './constants';
import Runtime from './runtime';

const bundleDirPath = path.resolve(__dirname, '..', 'bundle');
const examplesDirPath = path.resolve(__dirname, '..', 'examples');
const tmpDirPath = path.resolve(__dirname, '..', 'tmp');
const srcDirPath = path.resolve(__dirname, '..', 'src');

//
// Convert runtime
//
const convertRuntimeReplacers = map(flag =>
  replace(
    new RegExp(`^var ?${flag} ?= ?\\d+;`, 'm'),
    `var ${flag} = Module.${flag};`
  )
)([
  'ALLOC_DYNAMIC',
  'ALLOC_NONE',
  'ALLOC_NORMAL',
  'ALLOC_STACK',
  'ALLOC_STATIC',
  'ASMJS_PAGE_SIZE',
  'GLOBAL_BASE',
  'MIN_TOTAL_MEMORY',
  'PAGE_SIZE',
  'STACK_ALIGN',
  'STATIC_BUMP',
  'WASM_PAGE_SIZE',
]);

const convertRuntime = (src, dest) =>
  fs
    .readFile(src)
    .then(
      compose(
        concat(
          `/*XODMOD\n` +
            `  XODMOD  Tabtest runtime\n` +
            `  XODMOD\n` +
            `  XODMOD  Version: 1\n` +
            `  XODMOD  used compiler: ${CXX}\n` +
            `  XODMOD  compiler flags: ${CXX_FLAGS}\n` +
            `  XODMOD  linker flags: ${CXX_FINAL_FLAGS}\n` +
            `  XODMOD  based on compiled tabtest @/log-10\n` +
            `  XODMOD\n` +
            `  XODMOD  All modified lines marked with tag XODMOD\n` +
            `  XODMOD*/\n`
        ),
        replace(
          new RegExp(`^STATICTOP ?= ?STATIC_BASE ?\\+ ?\\d+;`, 'm'),
          `/* XODMOD */ STATICTOP = STATIC_BASE + Module.MAX_GLOBAL_ALIGN;`
        ),
        replace(
          /^Module\['(wasmTableSize|wasmMaxTableSize)'\] ?= ?\d+;/gm,
          '/* XODMOD */'
        ),
        apply(compose, convertRuntimeReplacers),
        replace(
          /__ATINIT__.push\(.*\);/m,
          `/* XODMOD global initializers */  __ATINIT__.push({ func: function() { __GLOBAL__I_000101() } }, { func: function() { __GLOBAL__sub_I_XODMAIN_cpp() } }, { func: function() { __GLOBAL__sub_I_main_cpp() } }, { func: function() { __GLOBAL__sub_I_iostream_cpp() } });`
        ),
        buf => buf.toString('utf8')
      )
    )
    .then(data => fs.writeFile(dest, data));

//
// Compilation
//
const compileToBc = (cwd, inPath, outPath) =>
  spawn(CXX, concat(split(' ')(CXX_FLAGS), ['-c', inPath, '-o', outPath]), {
    cwd,
    stdio: 'inherit',
  });

const parseJsRuntime = (text, targets) =>
  compose(
    fromPairs,
    map(target => {
      const regexp = cond([
        [
          () => target === 'wasmTableSize' || target === 'wasmMaxTableSize',
          () => new RegExp(`^Module\\['${target}'\\] ?= ?(\\d+);`, 'm'),
        ],
        [
          equals('MAX_GLOBAL_ALIGN'),
          () => new RegExp(`^STATICTOP ?= ?STATIC_BASE ?\\+ ?(\\d+);`, 'm'),
        ],
        [T, () => new RegExp(`^var ?${target} ?= ?(\\d+);`, 'm')],
      ])(target);
      // console.log(text.match(regexp));
      return [target, parseInt((text.match(regexp) || [])[1], 10)];
    })
  )(targets);

const compileToWasm = async (cwd, objects) => {
  await spawn(
    CXX,
    concat(split(' ')(CXX_FINAL_FLAGS), [
      ...objects,
      '-o',
      path.resolve(cwd, 'main.js'),
    ])
  );
  const wasm = await fs.readFile(path.resolve(cwd, 'main.wasm'));
  const js = (await fs.readFile(path.resolve(cwd, 'main.js'))).toString('utf8');

  return {
    ...parseJsRuntime(js, [
      'ALLOC_DYNAMIC',
      'ALLOC_NONE',
      'ALLOC_NORMAL',
      'ALLOC_STACK',
      'ALLOC_STATIC',
      'ASMJS_PAGE_SIZE',
      'GLOBAL_BASE',
      'MAX_GLOBAL_ALIGN',
      'MIN_TOTAL_MEMORY',
      'PAGE_SIZE',
      'STACK_ALIGN',
      'STATIC_BUMP',
      'WASM_PAGE_SIZE',
      'wasmTableSize',
      'wasmMaxTableSize',
    ]),
    wasmBinary: wasm,
  };
};

const compile = async (wd, files) => {
  if (!fs.existsSync(path.resolve(bundleDirPath, 'main.bc')))
    await compileToBc(
      bundleDirPath,
      path.resolve(bundleDirPath, 'main.cpp'),
      path.resolve(bundleDirPath, 'main.bc')
    );

  // stabilize entrypoint
  const sources = map(
    when(
      ([name]) => endsWith('.sketch.cpp')(name),
      ([, buf]) => ['XODMAIN.cpp', buf]
    )
  )(files);

  await fs.emptyDir(wd);
  await fs.copy(bundleDirPath, wd);
  await Promise.all(
    map(([name, buf]) => fs.writeFile(path.resolve(wd, name), buf))(sources)
  );
  await Promise.all(
    compose(
      map(async ([name]) => {
        const srcPath = path.resolve(wd, name);
        const bcPath = path.resolve(wd, `${name.match(/.*\./)}bc`);
        await compileToBc(wd, srcPath, bcPath);
      }),
      filter(([name]) => endsWith('.cpp')(toLower(path.extname(name))))
    )(sources)
  );

  const objects = compose(
    map(name => path.resolve(wd, name)),
    filter(endsWith('.bc'))
  )(await fs.readdir(wd));
  return compileToWasm(wd, objects);
};

//
// Runner
//
const run = object => {
  const stdout = [];
  const stderr = [];
  return new Promise(resolve => {
    const Module = {
      ...object,
      noFSInit: false,
      noExitRuntime: false,
      arguments: ['-e'],
      onAbort: () => console.error(`fuck i'm dead`),
      print: msg => stdout.push(msg),
      printErr: msg => stderr.push(msg),
      postRun: () => resolve({ stdout, stderr, exitCode: 0 }),
      quit: status => resolve({ stdout, stderr, exitCode: status }),
    };
    console.log(Module);
    // eslint-disable-next-line no-new
    new Runtime(Module);
  });
};

// compile and run all that shit
convertRuntime(
  path.resolve(srcDirPath, 'runtime.orig.js'),
  path.resolve(srcDirPath, 'runtime.js')
)
  .then(() => fs.readdir(examplesDirPath))
  .then(
    compose(
      fromPairs,
      map(name => [
        name,
        map(filename => [
          filename,
          fs.readFileSync(path.resolve(examplesDirPath, filename)),
        ])([
          'Arduino.cpp',
          'Arduino.h',
          `${name}.catch.inl`,
          `${name}.sketch.cpp`,
        ]),
      ]),
      map(name => name.split('.')[0]),
      filter(endsWith('.sketch.cpp'))
    )
  )
  .then(async tests => {
    for (const test of Object.keys(tests)) {
      const tmpDir = await fs.mkdtemp(path.resolve(tmpDirPath, test));
      const wasmObj = await compile(tmpDir, tests[test]);
      console.log(`\n\n\nTabtest ${test}`);
      const module = new WebAssembly.Module(wasmObj.wasmBinary);
      const imp = WebAssembly.Module.imports(module);
      const exp = WebAssembly.Module.exports(module);
      console.log('Exports', exp.length, exp);
      console.log('Imports', imp.length, imp);
      const r = await run(wasmObj);
      console.log(r.stdout);
      console.log(r.stderr);
      console.log(r.exitCode);
      await fs.remove(tmpDir);
    }
  })
  .catch(err => {
    console.error(err);
    if (err.stdout) console.log(err.stdout.toString());
    if (err.stderr) console.error(err.stderr.toString());
  });

// setInterval(() => console.log('not dead'), 1000);

