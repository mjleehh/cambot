import Mousetrap from 'mousetrap'
import {
    decreaseSpeed,
    increaseSpeed,
    requestLowerVert,
    requestPitch,
    requestRoll,
    requestUpperVert,
    requestYaw, setHome,
    setNumSteps
} from './actions'
import {PitchDirection, RollDirection, YawDirection} from './types'
import {Dispatch} from './store'

export const YAW_LEFT_KEY = 'left'
export const YAW_RIGHT_KEY = 'right'
export const LOWER_VERT_UP_KEY = 'up'
export const LOWER_VERT_DOWN_KEY = 'down'
export const UPPER_VERT_UP_KEY = 'pageup'
export const UPPER_VERT_DOWN_KEY = 'pagedown'
export const ROLL_LEFT_KEY = 'a'
export const ROLL_RIGHT_KEY = 'd'
export const PITCH_UP_KEY = 'w'
export const PITCH_DOWN_KEY = 's'

export const STEPS_1_KEY = '1'
export const STEPS_10_KEY = '2'
export const STEPS_100_KEY = '3'
export const STEPS_1000_KEY = '4'

export const INCREASE_SPEED_KEY = '+'
export const DECREASE_SPEED_KEY = '-'

export const SET_HOME_KEY = '0'

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

    Mousetrap.bind(INCREASE_SPEED_KEY, () =>
        dispatch(increaseSpeed()))

    Mousetrap.bind(DECREASE_SPEED_KEY, () =>
        dispatch(decreaseSpeed()))

    Mousetrap.bind(SET_HOME_KEY, () =>
        dispatch(setHome()))
}