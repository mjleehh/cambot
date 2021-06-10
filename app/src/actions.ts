import {PitchDirection, RollDirection, YawDirection} from './rotation-types'

export type Action =
    | RequestYawAction
    | RequestLowerPitchAction
    | RequestUpperPitchAction
    | RequestRollAction
    | RequestPitchAction
    | SetNumStepsAction
    | SetSpeedAction

export const REQUEST_YAW = 'REQUEST_YAW'
export type RequestYawAction = {
    type: typeof REQUEST_YAW
    payload: YawDirection
}
export const requestYaw = (direction: YawDirection): RequestYawAction => ({
    type: REQUEST_YAW,
    payload: direction,
})

export const REQUEST_LOWER_PITCH = 'REQUEST_LOWER_PITCH'
export type RequestLowerPitchAction = {
    type: typeof REQUEST_LOWER_PITCH
    payload: PitchDirection
}
export const requestLowerPitch = (direction: PitchDirection): RequestLowerPitchAction => ({
    type: REQUEST_LOWER_PITCH,
    payload: direction,
})

export const REQUEST_UPPER_PITCH = 'REQUEST_UPPER_PITCH'
export type RequestUpperPitchAction = {
    type: typeof REQUEST_UPPER_PITCH
    payload: PitchDirection
}
export const requestUpperPitch = (direction: PitchDirection): RequestUpperPitchAction => ({
    type: REQUEST_UPPER_PITCH,
    payload: direction,
})

export const REQUEST_ROLL = 'REQUEST_ROLL'
export type RequestRollAction = {
    type: typeof REQUEST_ROLL
    payload: RollDirection
}
export const requestRoll = (direction: RollDirection): RequestRollAction => ({
    type: REQUEST_ROLL,
    payload: direction,
})

export const REQUEST_PITCH = 'REQUEST_PITCH'
export type RequestPitchAction = {
    type: typeof REQUEST_PITCH
    payload: PitchDirection
}
export const requestPitch = (direction: PitchDirection): RequestPitchAction => ({
    type: REQUEST_PITCH,
    payload: direction,
})

export const SET_NUM_STEPS = 'SET_NUM_STEPS'
export type SetNumStepsAction = {
    type: typeof SET_NUM_STEPS
    payload: number
}
export const setNumSteps = (numSteps: number): SetNumStepsAction => ({
    type: SET_NUM_STEPS,
    payload: numSteps
})

export const SET_SPEED = 'SET_SPEED'
export type SetSpeedAction = {
    type: typeof SET_SPEED
    payload: number
}
export const setSpeed = (speed: number): SetSpeedAction => ({
    type: SET_SPEED,
    payload: speed
})