import { assert } from 'chai';

import {
  convertPixelsToSlots,
  convertPositionToPixels,
  convertSizeToPixels,
} from '../src/internal/dimensionConverters';
import { UNITS } from '../src/nodeLayout';

describe('Dimension converters', () => {
  it('convertPixelsToSlots', () => {
    const OLD = 34;
    assert.equal(convertPixelsToSlots(OLD, 0), 0);

    assert.equal(convertPixelsToSlots(OLD, 34), 1);
    assert.equal(convertPixelsToSlots(OLD, 33), 1);
    assert.equal(convertPixelsToSlots(OLD, 35), 1);

    assert.equal(convertPixelsToSlots(OLD, 36), 1.05);
    assert.equal(convertPixelsToSlots(OLD, 32), 0.95);
    assert.equal(convertPixelsToSlots(OLD, 30), 0.9);

    assert.equal(convertPixelsToSlots(OLD, -34), -1);
    assert.equal(convertPixelsToSlots(OLD, -30), -0.9);

    assert.equal(convertPixelsToSlots(OLD, 204), 6);
    assert.equal(convertPixelsToSlots(OLD, 190), 5.6);
    assert.equal(convertPixelsToSlots(OLD, -260), -7.65);
  });
  it('convertPositionToPixels', () => {
    assert.deepEqual(
      convertPositionToPixels({ x: 0, y: 0, units: UNITS.SLOTS }),
      {
        x: 0,
        y: 0,
        units: UNITS.PIXELS,
      }
    );
    assert.deepEqual(
      convertPositionToPixels({ x: 1, y: 2, units: UNITS.SLOTS }),
      {
        x: 44,
        y: 210,
        units: UNITS.PIXELS,
      }
    );
    assert.deepEqual(
      convertPositionToPixels({ x: -3.5, y: -9.25, units: UNITS.SLOTS }),
      {
        x: -154,
        y: -971,
        units: UNITS.PIXELS,
      }
    );

    assert.deepEqual(
      convertPositionToPixels({ x: 44, y: 105, units: UNITS.PIXELS }),
      {
        x: 44,
        y: 105,
        units: UNITS.PIXELS,
      }
    );
  });
  it('convertSizeToPixels', () => {
    assert.deepEqual(
      convertSizeToPixels({ width: 0, height: 0, units: UNITS.SLOTS }),
      {
        width: 0,
        height: 0,
        units: UNITS.PIXELS,
      }
    );
    assert.deepEqual(
      convertSizeToPixels({ width: 1, height: 1, units: UNITS.SLOTS }),
      {
        width: 44,
        height: 65,
        units: UNITS.PIXELS,
      }
    );
    assert.deepEqual(
      convertSizeToPixels({ width: 2.55, height: 5.75, units: UNITS.SLOTS }),
      {
        width: 113,
        height: 564,
        units: UNITS.PIXELS,
      }
    );

    assert.deepEqual(
      convertSizeToPixels({ width: 320, height: 510, units: UNITS.PIXELS }),
      {
        width: 320,
        height: 510,
        units: UNITS.PIXELS,
      }
    );
  });
});
