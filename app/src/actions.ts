import {PitchDirection, RollDirection, YawDirection, ArmState} from './types'

export type Action =
    | RequestYawAction
    | RequestLowerVertAction
    | RequestUpperVertAction
    | RequestRollAction
    | RequestPitchAction
    | SetNumStepsAction
    | SetSpeedAction
    | SetHomeAction
    | GotArmState
    | IncreaseSpeedAction
    | DecreaseSpeedAction
    | GoHomeAction
    | MoveToAction

export const REQUEST_YAW = 'REQUEST_YAW'
export type RequestYawAction = {
    type: typeof REQUEST_YAW
    payload: YawDirection
}
export const requestYaw = (direction: YawDirection): RequestYawAction => ({
    type: REQUEST_YAW,
    payload: direction,
})

export const REQUEST_LOWER_VERT = 'REQUEST_LOWER_PITCH'
export type RequestLowerVertAction = {
    type: typeof REQUEST_LOWER_VERT
    payload: PitchDirection
}
export const requestLowerVert = (direction: PitchDirection): RequestLowerVertAction => ({
    type: REQUEST_LOWER_VERT,
    payload: direction,
})

export const REQUEST_UPPER_VERT = 'REQUEST_UPPER_PITCH'
export type RequestUpperVertAction = {
    type: typeof REQUEST_UPPER_VERT
    payload: PitchDirection
}
export const requestUpperVert = (direction: PitchDirection): RequestUpperVertAction => ({
    type: REQUEST_UPPER_VERT,
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

export const GO_HOME = 'GO_HOME'
export type GoHomeAction = {
    type: typeof GO_HOME
}
export const goHome = (): GoHomeAction => ({
    type: GO_HOME
})

export const SET_HOME = 'SET_HOME'
export type SetHomeAction = {
    type: typeof SET_HOME
}
export const setHome = (): SetHomeAction => ({
    type: SET_HOME
})

export const GOT_ARM_STATE = 'GOT_ARM_STATE'
export type GotArmState = {
    type: typeof GOT_ARM_STATE
    payload: ArmState
}
export const gotArmState = (armState: ArmState): GotArmState => ({
    type: GOT_ARM_STATE,
    payload: armState,
})

export const INCREASE_SPEED = 'INCREASE_SPEED'
export type IncreaseSpeedAction = {
    type: typeof INCREASE_SPEED
}
export const increaseSpeed = (): IncreaseSpeedAction => ({
    type: INCREASE_SPEED,
})

export const DECREASE_SPEED = 'DECREASE_SPEED'
export type DecreaseSpeedAction = {
    type: typeof DECREASE_SPEED
}
export const decreaseSpeed = (): DecreaseSpeedAction => ({
    type: DECREASE_SPEED,
})

export const MOVE_YAW = 'MOVE_YAW'
export type MoveYawAction = {
    type: typeof MOVE_YAW
    payload: number
}
export const moveYaw = (angle: number): MoveYawAction => ({
    type: MOVE_YAW,
    payload: angle
})

export const MOVE_TO = 'MOVE_TO'
export type MoveToAction = {
    type: typeof MOVE_TO
    payload: {
        x: number
        y: number
        z: number
        speed: number
    }
}
export const moveTo = (x: number, y: number, z: number, speed: number): MoveToAction => ({
    type: MOVE_TO,
    payload: {x, y, z, speed}
})
