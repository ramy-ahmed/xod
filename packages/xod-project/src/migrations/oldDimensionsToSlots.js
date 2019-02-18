import R from 'ramda';

import { def } from '../types';
import * as Project from '../project';
import * as Patch from '../patch';
import * as Node from '../node';
import * as Comment from '../comment';

import {
  setSlotUnits,
  hasUnits,
  convertPixelsToSlots,
} from '../internal/dimensionConverters';

import { SLOT_SIZE } from '../nodeLayout';

/* eslint-enable new-cap */
//-----------------------------------------------------------------------------
//
// Constants
//
//-----------------------------------------------------------------------------

// Grid until XOD v0.27.0
const LEGACY_SLOT_GRID = {
  WIDTH: 34,
  HEIGHT: 102,
  GAP: 51,
};

//-----------------------------------------------------------------------------
//
// Private API
//
//-----------------------------------------------------------------------------

const migratePosition = def(
  'migratePosition :: Position -> Position',
  R.unless(
    hasUnits,
    R.compose(
      setSlotUnits,
      R.evolve({
        x: convertPixelsToSlots(LEGACY_SLOT_GRID.WIDTH),
        y: convertPixelsToSlots(LEGACY_SLOT_GRID.HEIGHT),
      })
    )
  )
);

const migrateSize = def(
  'migrateSize :: Size -> Size',
  R.unless(
    hasUnits,
    R.compose(
      setSlotUnits,
      R.evolve({
        width: w => Math.ceil(w / SLOT_SIZE.WIDTH),
        height: R.when(
          h => h !== 0,
          h => Math.ceil((h + LEGACY_SLOT_GRID.GAP) / SLOT_SIZE.HEIGHT)
        ),
      })
    )
  )
);

const migrateNodeDimensionsToSlots = def(
  'migrateNodeDimensionsToSlots :: Node -> Node',
  node => {
    const position = R.compose(migratePosition, Node.getNodePosition)(node);
    const size = R.compose(migrateSize, Node.getNodeSize)(node);
    return R.compose(R.assoc('size', size), R.assoc('position', position))(
      node
    );
  }
);

const migrateCommentDimensionsToSlots = def(
  'migrateCommentDimensionsToSlots :: Comment -> Comment',
  comment => {
    const position = R.compose(migratePosition, Comment.getCommentPosition)(
      comment
    );
    const size = R.compose(migrateSize, Comment.getCommentSize)(comment);
    return R.compose(R.assoc('size', size), R.assoc('position', position))(
      comment
    );
  }
);

//-----------------------------------------------------------------------------
//
// Public API
//
//-----------------------------------------------------------------------------

/**
 * Returns Project with converted old pixels into slots of
 * Nodes' and Comments' positions and sizes.
 */
export default def(
  'migrateOldDimensionsToSlots :: Project -> Project',
  project =>
    R.compose(
      R.reduce((accProject, patch) => {
        const patchPath = Patch.getPatchPath(patch);
        const nodes = R.compose(
          R.map(migrateNodeDimensionsToSlots),
          Patch.listNodes
        )(patch);
        const comments = R.compose(
          R.map(migrateCommentDimensionsToSlots),
          Patch.listComments
        )(patch);

        return R.compose(
          Project.assocPatch(patchPath, R.__, accProject),
          Patch.upsertComments(comments),
          Patch.upsertNodes(nodes)
        )(patch);
      }, project),
      Project.listGenuinePatches
    )(project)
);
