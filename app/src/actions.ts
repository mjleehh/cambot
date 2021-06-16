import {
    PitchDirection,
    RollDirection,
    YawDirection,
    ArmState,
    Move,
    DirectionForward,
    DirectionHorizontal, DirectionVertical
} from './types'

export type Action =
    | RequestYawAction
    | RequestLowerVertAction
    | RequestUpperVertAction
    | RequestRollAction
    | RequestPitchAction
    | SetNumStepsAction
    | SetRotationSpeedAction
    | SetHomeAction
    | GotArmState
    | IncreaseRotationSpeedAction
    | DecreaseRotationSpeedAction
    | GoHomeAction
    | MoveToAction
    | RequestMoveAction
    | RequestStopAction
    | SetDistanceAction
    | SetSpeedAction
    | RequestMoveForwardAction
    | RequestMoveHorizontalAction
    | RequestMoveVerticalAction
    | IncreaseSpeedAction
    | DecreaseSpeedAction


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

export const SET_ROTATION_SPEED = 'SET_ROTATION_SPEED'
export type SetRotationSpeedAction = {
    type: typeof SET_ROTATION_SPEED
    payload: number
}
export const setRotationSpeed = (speed: number): SetRotationSpeedAction => ({
    type: SET_ROTATION_SPEED,
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

export const INCREASE_ROTATION_SPEED = 'DECREASE_ROTATION_SPEED'
export type IncreaseRotationSpeedAction = {
    type: typeof INCREASE_ROTATION_SPEED
}
export const increaseRotationSpeed = (): IncreaseRotationSpeedAction => ({
    type: INCREASE_ROTATION_SPEED,
})

export const DECREASE_ROTATION_SPEED = 'INCREASE_ROTATION_SPEED'
export type DecreaseRotationSpeedAction = {
    type: typeof DECREASE_ROTATION_SPEED
}
export const decreaseRotationSpeed = (): DecreaseRotationSpeedAction => ({
    type: DECREASE_ROTATION_SPEED,
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

export const REQUEST_MOVE = 'REQUEST_MOVE'
export type RequestMoveAction = {
    type: typeof REQUEST_MOVE
    payload: Move
}
export const requestMove = (forward: number, horizontal: number, vertical: number): RequestMoveAction => ({
    type: REQUEST_MOVE,
    payload: {forward, horizontal, vertical},
})

export const REQUEST_STOP = 'REQUEST_STOP'
export type RequestStopAction = {
    type: typeof REQUEST_STOP
}
export const requestStop = (): RequestStopAction => ({
    type: REQUEST_STOP
})

export const REQUEST_MOVE_FORWARD = 'REQUEST_MOVE_FORWARD'
export type RequestMoveForwardAction = {
    type: typeof REQUEST_MOVE_FORWARD
    payload: DirectionForward
}
export const requestMoveForward = (direction: DirectionForward): RequestMoveForwardAction => ({
    type: REQUEST_MOVE_FORWARD,
    payload: direction,
})

export const REQUEST_MOVE_HORIZONTAL = 'REQUEST_MOVE_HORIZONTAL'
export type RequestMoveHorizontalAction = {
    type: typeof REQUEST_MOVE_HORIZONTAL
    payload: DirectionHorizontal
}
export const requestMoveHorizontal = (direction: DirectionHorizontal): RequestMoveHorizontalAction => ({
    type: REQUEST_MOVE_HORIZONTAL,
    payload: direction,
})

export const REQUEST_MOVE_VERTICAL = 'REQUEST_MOVE_VERTICAL'
export type RequestMoveVerticalAction = {
    type: typeof REQUEST_MOVE_VERTICAL
    payload: DirectionVertical
}
export const requestMoveVertical = (direction: DirectionVertical): RequestMoveVerticalAction => ({
    type: REQUEST_MOVE_VERTICAL,
    payload: direction,
})

export const SET_DISTANCE = 'SET_DISTANCE'
export type SetDistanceAction = {
    type: typeof SET_DISTANCE
    payload: number
}
export const setDistance = (distance: number): SetDistanceAction => ({
    type: SET_DISTANCE,
    payload: distance,
})

export const SET_SPEED = 'SET_SPEED'
export type SetSpeedAction = {
    type: typeof SET_SPEED
    payload: number
}
export const setSpeed = (speed: number): SetSpeedAction => ({
    type: SET_SPEED,
    payload: speed,
})

export const INCREASE_SPEED = 'INCREASE_SPEED'
export type IncreaseSpeedAction = {
    type: typeof INCREASE_SPEED
}
export const increaseSpeed = (): IncreaseSpeedAction => ({
    type: INCREASE_SPEED
})

export const DECREASE_SPEED = 'DECREASE_SPEED'
export type DecreaseSpeedAction = {
    type: typeof DECREASE_SPEED
}
export const decreaseSpeed = (): DecreaseSpeedAction => ({
    type: DECREASE_SPEED
})
