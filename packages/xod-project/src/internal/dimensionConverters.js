import R from 'ramda';

import { def } from '../types';
import { SLOT_SIZE, UNITS } from '../nodeLayout';

//-----------------------------------------------------------------------------
//
// Constants
//
//-----------------------------------------------------------------------------

const ROUNDING_ERROR = 20; // 5%

//-----------------------------------------------------------------------------
//
// Utils
//
//-----------------------------------------------------------------------------

export const setSlotUnits = R.assoc('units', UNITS.SLOTS);

export const isSlotUnits = def(
  'isSlotUnits :: Object -> Boolean',
  R.propEq('units', UNITS.SLOTS)
);

export const isPixelUnits = def(
  'isPixelUnits :: Object -> Boolean',
  R.propEq('units', UNITS.PIXELS)
);

export const hasUnits = def(
  'hasUnits :: Object -> Boolean',
  R.either(isSlotUnits, isPixelUnits)
);

export const convertPixelsToSlots = def(
  'convertPixelsToSlots :: Number -> Number -> Number',
  (slotSize, px) => {
    const ratio = px / slotSize;
    const roundFn = ratio % 1 > 0.5 ? Math.ceil : Math.floor;
    return roundFn(px / slotSize * ROUNDING_ERROR) / ROUNDING_ERROR;
  }
);

//-----------------------------------------------------------------------------
//
// Converters
//
//-----------------------------------------------------------------------------

export const convertPositionToPixels = def(
  'convertPositionToPixels :: Position -> Position',
  R.when(
    isSlotUnits,
    R.evolve({
      x: x => Math.ceil(x * SLOT_SIZE.WIDTH),
      y: y => Math.ceil(y * SLOT_SIZE.HEIGHT),
      units: R.always(UNITS.PIXELS),
    })
  )
);

export const convertPositionToSlots = def(
  'convertPositionToSlots :: Position -> Position',
  R.unless(
    isSlotUnits,
    R.compose(
      setSlotUnits,
      R.evolve({
        x: convertPixelsToSlots(SLOT_SIZE.WIDTH),
        y: convertPixelsToSlots(SLOT_SIZE.HEIGHT),
      })
    )
  )
);

export const convertSizeToPixels = def(
  'convertSizeToPixels :: Size -> Size',
  R.when(
    isSlotUnits,
    R.evolve({
      width: w => Math.ceil(w * SLOT_SIZE.WIDTH),
      height: R.when(
        h => h !== 0,
        h => Math.ceil(h * SLOT_SIZE.HEIGHT) - SLOT_SIZE.GAP
      ),
      units: R.always(UNITS.PIXELS),
    })
  )
);

export const convertSizeToSlots = def(
  'convertSizeToSlots :: Size -> Size',
  R.unless(
    isSlotUnits,
    R.compose(
      setSlotUnits,
      R.evolve({
        width: convertPixelsToSlots(SLOT_SIZE.WIDTH),
        height: R.when(
          h => h !== 0,
          R.pipe(R.add(SLOT_SIZE.GAP), convertPixelsToSlots(SLOT_SIZE.HEIGHT))
        ),
      })
    )
  )
);

//-----------------------------------------------------------------------------
//
// Dimension normalizers
//
//-----------------------------------------------------------------------------
export const normalizePosition = def(
  'normalizePosition :: Position -> Position',
  R.ifElse(isSlotUnits, convertPositionToPixels, R.assoc('units', UNITS.PIXELS))
);

export const normalizeSize = def(
  'normalizeSize :: Size -> Size',
  R.ifElse(isSlotUnits, convertSizeToPixels, R.assoc('units', UNITS.PIXELS))
);
