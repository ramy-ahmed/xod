import * as R from 'ramda';
import { assert } from 'chai';
import { Either } from 'ramda-fantasy';
import {
  validateSanctuaryType,
  foldEither,
  explodeEither,
  fail,
} from 'xod-func-tools';

import { Project as $Project } from '../src/types';
import { addMissingOptionalProjectFields } from '../src/optionalFieldsUtils';
import { UNITS } from '../src/nodeLayout';
import migrateOldDimensionsToSlots from '../src/migrations/oldDimensionsToSlots';
import * as Project from '../src/project';
import * as Patch from '../src/patch';
import * as Node from '../src/node';
import * as Comment from '../src/comment';

import * as Helper from './helpers';

describe('Migration: old dimensions to slots', () => {
  // Special Helper to load xodball, BUT
  // without migrating and converting dimenstions,
  // because that's what we want to test
  const loadXodballWithoutConversion = R.compose(
    explodeEither,
    // Next code is a copy of `fromXodballData` without migration and converting
    R.map(Project.injectProjectTypeHints),
    foldEither(() => fail('INVALID_XODBALL_FORMAT', {}), Either.of),
    validateSanctuaryType($Project),
    addMissingOptionalProjectFields,
    // End of the part from `fromXodballData`
    Helper.loadJSON
  );

  it('on Blinking project', () => {
    const project = loadXodballWithoutConversion('./fixtures/blinking.xodball');
    const convertedProject = migrateOldDimensionsToSlots(project);

    // Check Nodes' Position
    const actualNodePositions = R.compose(
      R.map(Node.getNodePosition),
      Patch.listNodes,
      Project.getPatchByPathUnsafe('@/blink')
    )(convertedProject);
    const expectedNodePositions = [
      {
        x: 7.75,
        y: 5,
        units: UNITS.SLOTS,
      },
      {
        x: 8,
        y: 3.45,
        units: UNITS.SLOTS,
      },
      {
        x: 7.9,
        y: 2.1,
        units: UNITS.SLOTS,
      },
      {
        x: 7.9,
        y: 1,
        units: UNITS.SLOTS,
      },
    ];
    assert.sameDeepMembers(actualNodePositions, expectedNodePositions);

    // Check Comments' Position
    const actualCommentPositions = R.compose(
      R.map(Comment.getCommentPosition),
      Patch.listComments,
      Project.getPatchByPathUnsafe('@/blink')
    )(convertedProject);
    const expectedCommentPositions = [
      {
        x: 1.25,
        y: 1,
        units: UNITS.SLOTS,
      },
    ];
    assert.sameDeepMembers(actualCommentPositions, expectedCommentPositions);

    // Check Comments' Size
    const actualCommentSize = R.compose(
      R.map(Comment.getCommentSize),
      Patch.listComments,
      Project.getPatchByPathUnsafe('@/blink')
    )(convertedProject);
    const expectedCommentSize = [
      {
        width: 4,
        height: 2,
        units: UNITS.SLOTS,
      },
    ];
    assert.sameDeepMembers(actualCommentSize, expectedCommentSize);
  });
});
