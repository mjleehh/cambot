import {PitchDirection, RollDirection, YawDirection} from './rotation-types'

export type Action =
    | RequestYawAction
    | RequestLowerPitchAction
    | RequestUpperPitchAction
    | RequestRollAction
    | RequestPitchAction

export const REQUEST_YAW = 'REQUEST_YAW'
export type RequestYawAction = {
    type: typeof REQUEST_YAW
    payload: {
        direction: YawDirection
        steps: number
        speed: number

    }
}
export const requestYaw = (direction: YawDirection, steps: number, speed: number): RequestYawAction => ({
    type: REQUEST_YAW,
    payload: {
        direction,
        steps,
        speed,
    }
})

export const REQUEST_LOWER_PITCH = 'REQUEST_LOWER_PITCH'
export type RequestLowerPitchAction = {
    type: typeof REQUEST_LOWER_PITCH
    payload: {
        direction:PitchDirection
        steps: number
        speed: number

    }
}
export const requestLowerPitch = (direction: PitchDirection, steps: number, speed: number): RequestLowerPitchAction => ({
    type: REQUEST_LOWER_PITCH,
    payload: {
        direction,
        steps,
        speed,
    }
})

export const REQUEST_UPPER_PITCH = 'REQUEST_UPPER_PITCH'
export type RequestUpperPitchAction = {
    type: typeof REQUEST_UPPER_PITCH
    payload: {
        direction:PitchDirection
        steps: number
        speed: number

    }
}
export const requestUpperPitch = (direction: PitchDirection, steps: number, speed: number): RequestUpperPitchAction => ({
    type: REQUEST_UPPER_PITCH,
    payload: {
        direction,
        steps,
        speed,
    }
})

export const REQUEST_ROLL = 'REQUEST_ROLL'
export type RequestRollAction = {
    type: typeof REQUEST_ROLL
    payload: {
        direction:RollDirection
        steps: number
        speed: number

    }
}
export const requestRoll = (direction: RollDirection, steps: number, speed: number): RequestRollAction => ({
    type: REQUEST_ROLL,
    payload: {
        direction,
        steps,
        speed,
    }
})

export const REQUEST_PITCH = 'REQUEST_PITCH'
export type RequestPitchAction = {
    type: typeof REQUEST_PITCH
    payload: {
        direction:PitchDirection
        steps: number
        speed: number

    }
}
export const requestPitch = (direction: PitchDirection, steps: number, speed: number): RequestPitchAction => ({
    type: REQUEST_PITCH,
    payload: {
        direction,
        steps,
        speed,
    }
})