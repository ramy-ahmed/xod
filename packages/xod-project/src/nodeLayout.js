// Once we'll fully migrate to the slot system,
// then clients can decide how to render slots.
// But until then, we have to know sizes here...

export const BASE_SIZE_UNIT = 5;

export const NODE_HEIGHT = BASE_SIZE_UNIT * 13;

const GAP_BETWEEN_LINES = BASE_SIZE_UNIT * 8;

export const SLOT_SIZE = {
  WIDTH: BASE_SIZE_UNIT * 8 + 4,
  HEIGHT: NODE_HEIGHT + GAP_BETWEEN_LINES,
  GAP: GAP_BETWEEN_LINES,
};

export const UNITS = {
  PIXELS: 'pixels',
  SLOTS: 'slots',
};
