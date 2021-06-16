import {
    Action,
    DECREASE_ROTATION_SPEED, DECREASE_SPEED,
    GOT_ARM_STATE,
    INCREASE_ROTATION_SPEED, INCREASE_SPEED, SET_DISTANCE,
    SET_NUM_STEPS,
    SET_ROTATION_SPEED,
    SET_SPEED
} from './actions'
import {ArmState} from './types'

export type State = {
    numSteps: number
    rotationSpeed: number
    speed: number
    distance: number
    armState: ArmState
}

function initialState(): State {
    return {
        numSteps: 1,
        rotationSpeed: 500,
        speed: 1,
        distance: 10,
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
        case SET_ROTATION_SPEED: {
            return {...state, rotationSpeed: payload as number}
        }
        case GOT_ARM_STATE: {
            return {...state, armState: payload as ArmState}
        }
        case INCREASE_ROTATION_SPEED: {
            return {...state, rotationSpeed: state.rotationSpeed * 1.1}
        }
        case DECREASE_ROTATION_SPEED: {
            return {...state, rotationSpeed: state.rotationSpeed * 0.9}
        }
        case SET_SPEED: {
            return {...state, speed: payload}
        }
        case SET_DISTANCE: {
            return {...state, distance: payload}
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