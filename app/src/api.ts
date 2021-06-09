import {PitchDirection, RollDirection, YawDirection} from './rotation-types'
import axios from 'axios'

class Api {
    yaw(direction: YawDirection, steps: number, speed: number) {
        if (direction === YawDirection.left) {
            return axios.post(`/api/yaw/left/${steps}/${speed}`)
        } else if (direction === YawDirection.right) {
            return axios.post(`/api/yaw/right/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    lowerPitch(direction: PitchDirection, steps: number, speed: number) {
        if (direction === PitchDirection.up) {
            return axios.post(`/api/lv/up/${steps}/${speed}`)
        } else if (direction === PitchDirection.down) {
            return axios.post(`/api/lv/down/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    upperPitch(direction: PitchDirection, steps: number, speed: number) {
        if (direction === PitchDirection.up) {
            return axios.post(`/api/uv/up/${steps}/${speed}`)
        } else if (direction === PitchDirection.down) {
            return axios.post(`/api/uv/down/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    roll(direction: RollDirection, steps: number, speed: number) {
        if (direction === RollDirection.ccw) {
            return axios.post(`/api/roll/ccw/${steps}/${speed}`)
        } else if (direction === RollDirection.cw) {
            return axios.post(`/api/roll/cw/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    pitch(direction: PitchDirection, steps: number, speed: number) {
        if (direction === PitchDirection.up) {
            return axios.post(`/api/pitch/up/${steps}/${speed}`)
        } else if (direction === PitchDirection.down) {
            return axios.post(`/api/pitch/down/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }
}
const api = new Api()
export default api
