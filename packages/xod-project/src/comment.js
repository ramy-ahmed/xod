import * as R from 'ramda';
import * as Utils from './utils';
import { def } from './types';

import {
  normalizePosition,
  normalizeSize,
  convertPositionToPixels,
  convertPositionToSlots,
  convertSizeToPixels,
  convertSizeToSlots,
} from './internal/dimensionConverters';

export const createComment = def(
  'createComment :: Position -> Size -> String -> Comment',
  (position, size, content) => ({
    id: Utils.generateId(),
    position: normalizePosition(position),
    size: normalizeSize(size),
    content,
  })
);

export const getCommentId = def(
  'getCommentId :: Comment -> CommentId',
  R.prop('id')
);

export const setCommentId = def(
  'setCommentId :: CommentId -> Comment -> Comment',
  R.assoc('id')
);

export const getCommentPosition = def(
  'getCommentPosition :: Comment -> Position',
  R.prop('position')
);

export const setCommentPosition = def(
  'setCommentPosition :: Position -> Comment -> Comment',
  (position, comment) =>
    R.compose(R.assoc('position', R.__, comment), normalizePosition)(position)
);

export const getCommentSize = def(
  'getCommentSize :: Comment -> Size',
  R.prop('size')
);

export const setCommentSize = def(
  'setCommentSize :: Size -> Comment -> Comment',
  (size, comment) =>
    R.compose(R.assoc('size', R.__, comment), normalizeSize)(size)
);

export const getCommentContent = def(
  'getCommentContent :: Comment -> String',
  R.prop('content')
);

export const setCommentContent = def(
  'setCommentContent :: String -> Comment -> Comment',
  R.assoc('content')
);

/**
 * Returns Comment with converted Position and Size properties into slot units.
 */
export const convertCommentDimensionsToSlots = def(
  'convertCommentDimensionsToSlots :: Node -> Node',
  comment => {
    const position = R.compose(convertPositionToSlots, getCommentPosition)(
      comment
    );
    const size = R.compose(convertSizeToSlots, getCommentSize)(comment);
    return R.compose(R.assoc('size', size), R.assoc('position', position))(
      comment
    );
  }
);

/**
 * Returns Comment with converted Position and Size properties into pixel units.
 */
export const convertCommentDimensionsToPixels = def(
  'convertCommentDimensionsToPixels :: Comment -> Comment',
  comment => {
    const position = R.compose(convertPositionToPixels, getCommentPosition)(
      comment
    );
    const size = R.compose(convertSizeToPixels, getCommentSize)(comment);
    return R.compose(R.assoc('size', size), R.assoc('position', position))(
      comment
    );
  }
);
