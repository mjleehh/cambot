import {ArmState, PitchDirection, RollDirection, YawDirection} from './types'
import axios from 'axios'

class Api {
    yaw(direction: YawDirection, steps: number, speed: number) {
        if (direction === YawDirection.left) {
            return axios.post(`/api/calibrate/yaw/left/${steps}/${speed}`)
        } else if (direction === YawDirection.right) {
            return axios.post(`/api/calibrate/yaw/right/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    lowerVert(direction: PitchDirection, steps: number, speed: number) {
        if (direction === PitchDirection.up) {
            return axios.post(`/api/calibrate/lv/up/${steps}/${speed}`)
        } else if (direction === PitchDirection.down) {
            return axios.post(`/api/calibrate/lv/down/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    upperVert(direction: PitchDirection, steps: number, speed: number) {
        if (direction === PitchDirection.up) {
            return axios.post(`/api/calibrate/uv/up/${steps}/${speed}`)
        } else if (direction === PitchDirection.down) {
            return axios.post(`/api/calibrate/uv/down/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    roll(direction: RollDirection, steps: number, speed: number) {
        if (direction === RollDirection.ccw) {
            return axios.post(`/api/calibrate/roll/ccw/${steps}/${speed}`)
        } else if (direction === RollDirection.cw) {
            return axios.post(`/api/calibrate/roll/cw/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    pitch(direction: PitchDirection, steps: number, speed: number) {
        if (direction === PitchDirection.up) {
            return axios.post(`/api/calibrate/pitch/up/${steps}/${speed}`)
        } else if (direction === PitchDirection.down) {
            return axios.post(`/api/calibrate/pitch/down/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    goHome(): Promise<void> {
        return axios.post('/api/home')
    }

    setHome(): Promise<void> {
        return axios.post('/api/calibrate/set-home')
    }

    async armState(): Promise<ArmState> {
        const {data} = await axios.get('/api/state')
        return data
    }

    moveYaw(angle: number, speed: number) {
        return axios.post(`/api/move/yaw/${angle}/${speed}`)
    }

    moveTo(x: number, y: number, z: number, speed: number): Promise<void> {
        return axios.post(`/api/move/position/${x}/${y}/${z}/${speed}`)
    }
}
const api = new Api()
export default api
