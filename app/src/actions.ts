import {PitchDirection, YawDirection} from './rotation-types'

export type Action = RequestYawAction | RequestUpperPitchAction

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