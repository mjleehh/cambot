import {Action, DECREASE_SPEED, GOT_ARM_STATE, INCREASE_SPEED, SET_NUM_STEPS, SET_SPEED} from './actions'
import {ArmState} from './types'

export type State = {
    numSteps: number
    speed: number
    armState: ArmState
}

function initialState(): State {
    return {
        numSteps: 1,
        speed: 500,
        armState: {
            rotations: {
                yaw: {deg: NaN, steps: NaN},
                lowerVert: {deg: NaN, steps: NaN},
                upperVert: {deg: NaN, steps: NaN},
                roll: {deg: NaN, steps: NaN},
                pitch: {deg: NaN, steps: NaN},
            },
            position: {
                x: NaN,
                y: NaN,
                z: NaN,
            },
        }
    }
}

export default function reducer(state: State = initialState(), {type, payload}: Action): State {
    switch (type) {
        case SET_NUM_STEPS: {
            return {...state, numSteps: payload as number}
        }
        case SET_SPEED: {
            return {...state, speed: payload as number}
        }
        case GOT_ARM_STATE: {
            return {...state, armState: payload as ArmState}
        }
        case INCREASE_SPEED: {
            return {...state, speed: state.speed * 1.1}
        }
        case DECREASE_SPEED: {
            return {...state, speed: state.speed * 0.9}
        }
        default:
            return state
    }
}