import {PitchDirection, YawDirection} from './rotation-types'
import axios from 'axios'

class Api {
    yaw(direction: YawDirection, steps: number, speed: number) {
        if (direction === YawDirection.cw) {
            return axios.post(`/api/yaw/left/${steps}/${speed}`)
        } else if (direction === YawDirection.ccw) {
            return axios.post(`/api/yaw/right/${steps}/${speed}`)
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
}
const api = new Api()
export default api
