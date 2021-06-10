import Mousetrap from 'mousetrap'
import {requestLowerPitch, requestPitch, requestRoll, requestUpperPitch, requestYaw} from './actions'
import {PitchDirection, RollDirection, YawDirection} from './rotation-types'
import {Dispatch} from './store'

export default function bindKeys(dispatch: Dispatch) {
    Mousetrap.bind('left', () =>
        dispatch(requestYaw(YawDirection.left)))

    Mousetrap.bind('right', () =>
        dispatch(requestYaw(YawDirection.right)))

    Mousetrap.bind('pageup', () =>
        dispatch(requestUpperPitch(PitchDirection.up)))

    Mousetrap.bind('pagedown', () =>
        dispatch(requestUpperPitch(PitchDirection.down)))

    Mousetrap.bind('up', () =>
        dispatch(requestLowerPitch(PitchDirection.up)))

    Mousetrap.bind('down', () =>
        dispatch(requestLowerPitch(PitchDirection.down)))

    Mousetrap.bind('a', () =>
        dispatch(requestRoll(RollDirection.ccw)))

    Mousetrap.bind('d', () =>
        dispatch(requestRoll(RollDirection.cw)))

    Mousetrap.bind('w', () =>
        dispatch(requestPitch(PitchDirection.up)))

    Mousetrap.bind('s', () =>
        dispatch(requestPitch(PitchDirection.down)))
}