import {Action, SET_NUM_STEPS, SET_SPEED} from './actions'

export type State = {
    numSteps: number
    speed: number
}

function initialState(): State {
    return {
        numSteps: 1,
        speed: 500,
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
        default:
            return state
    }
}