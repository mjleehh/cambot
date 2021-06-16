import Mousetrap from 'mousetrap'
import {
    decreaseRotationSpeed, decreaseSpeed,
    goHome,
    increaseRotationSpeed, increaseSpeed,
    requestLowerVert,
    requestMoveForward,
    requestMoveHorizontal,
    requestMoveVertical,
    requestPitch,
    requestRoll,
    requestStop,
    requestUpperVert,
    requestYaw, setDistance,
    setHome,
    setNumSteps,
} from './actions'
import {
    DirectionForward,
    DirectionHorizontal,
    DirectionVertical,
    PitchDirection,
    RollDirection,
    YawDirection
} from './types'
import {Dispatch} from './store'

// rotation
// individual rotations
export const YAW_LEFT_KEY = 'left'
export const YAW_RIGHT_KEY = 'right'
export const LOWER_VERT_UP_KEY = 'up'
export const LOWER_VERT_DOWN_KEY = 'down'
export const UPPER_VERT_UP_KEY = 'pageup'
export const UPPER_VERT_DOWN_KEY = 'pagedown'
export const ROLL_LEFT_KEY = 'j'
export const ROLL_RIGHT_KEY = 'l'
export const PITCH_UP_KEY = 'i'
export const PITCH_DOWN_KEY = 'k'

// rotation angle
export const STEPS_1_KEY = '7'
export const STEPS_10_KEY = '8'
export const STEPS_100_KEY = '9'
export const STEPS_1000_KEY = '0'

// rotation speed
export const INCREASE_ROTATION_SPEED_KEY = '+'
export const DECREASE_ROTATION_SPEED_KEY = '-'

//// movement
export const GO_HOME_KEY = 'h'
export const SET_HOME_KEY = 'enter'

export const STOP_KEY = 'space'
export const MOVE_LEFT_KEY = 'a'
export const MOVE_RIGHT_KEY = 'd'
export const MOVE_FORWARD_KEY = 'w'
export const MOVE_BACKWARD_KEY = 's'
export const MOVE_UP_KEY = 'q'
export const MOVE_DOWN_KEY = 'e'

export const DISTANCE_1MM_KEY = '1'
export const DISTANCE_10MM_KEY = '2'
export const DISTANCE_100MM_KEY = '3'

export const INCREASE_SPEED_KEY = 'r'
export const DECREASE_SPEED_KEY = 'f'

export default function bindKeys(dispatch: Dispatch): void {
    Mousetrap.bind(YAW_LEFT_KEY, () =>
        dispatch(requestYaw(YawDirection.left)))

    Mousetrap.bind(YAW_RIGHT_KEY, () =>
        dispatch(requestYaw(YawDirection.right)))

    Mousetrap.bind(LOWER_VERT_UP_KEY, () =>
        dispatch(requestLowerVert(PitchDirection.up)))

    Mousetrap.bind(LOWER_VERT_DOWN_KEY, () =>
        dispatch(requestLowerVert(PitchDirection.down)))

    Mousetrap.bind(UPPER_VERT_UP_KEY, () =>
        dispatch(requestUpperVert(PitchDirection.up)))

    Mousetrap.bind(UPPER_VERT_DOWN_KEY, () =>
        dispatch(requestUpperVert(PitchDirection.down)))

    Mousetrap.bind(ROLL_LEFT_KEY, () =>
        dispatch(requestRoll(RollDirection.ccw)))

    Mousetrap.bind(ROLL_RIGHT_KEY, () =>
        dispatch(requestRoll(RollDirection.cw)))

    Mousetrap.bind(PITCH_UP_KEY, () =>
        dispatch(requestPitch(PitchDirection.up)))

    Mousetrap.bind(PITCH_DOWN_KEY, () =>
        dispatch(requestPitch(PitchDirection.down)))

    Mousetrap.bind(STEPS_1_KEY, () =>
        dispatch(setNumSteps(1)))

    Mousetrap.bind(STEPS_10_KEY, () =>
        dispatch(setNumSteps(10)))

    Mousetrap.bind(STEPS_100_KEY, () =>
        dispatch(setNumSteps(100)))

    Mousetrap.bind(STEPS_1000_KEY, () =>
        dispatch(setNumSteps(1000)))

    Mousetrap.bind(INCREASE_ROTATION_SPEED_KEY, () =>
        dispatch(increaseRotationSpeed()))

    Mousetrap.bind(DECREASE_ROTATION_SPEED_KEY, () =>
        dispatch(decreaseRotationSpeed()))

    Mousetrap.bind(GO_HOME_KEY, () =>
        dispatch(goHome()))

    Mousetrap.bind(SET_HOME_KEY, () =>
        dispatch(setHome()))

    Mousetrap.bind(STOP_KEY, () =>
        dispatch(requestStop()))

    Mousetrap.bind(MOVE_FORWARD_KEY, () =>
        dispatch(requestMoveForward(DirectionForward.forward)))

    Mousetrap.bind(MOVE_BACKWARD_KEY, () =>
        dispatch(requestMoveForward(DirectionForward.backward)))

    Mousetrap.bind(MOVE_LEFT_KEY, () =>
        dispatch(requestMoveHorizontal(DirectionHorizontal.left)))

    Mousetrap.bind(MOVE_RIGHT_KEY, () =>
        dispatch(requestMoveHorizontal(DirectionHorizontal.right)))

    Mousetrap.bind(MOVE_UP_KEY, () =>
        dispatch(requestMoveVertical(DirectionVertical.up)))

    Mousetrap.bind(MOVE_DOWN_KEY, () =>
        dispatch(requestMoveVertical(DirectionVertical.down)))

    Mousetrap.bind(DISTANCE_1MM_KEY, () =>
        dispatch(setDistance(1)))

    Mousetrap.bind(DISTANCE_10MM_KEY, () =>
        dispatch(setDistance(10)))

    Mousetrap.bind(DISTANCE_100MM_KEY, () =>
        dispatch(setDistance(100)))

    Mousetrap.bind(INCREASE_SPEED_KEY, () =>
        dispatch(increaseSpeed()))

    Mousetrap.bind(DECREASE_SPEED_KEY, () =>
        dispatch(decreaseSpeed()))
}