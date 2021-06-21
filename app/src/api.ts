import {
    ArmState,
    DirectionForward,
    DirectionHorizontal,
    DirectionVertical,
    PitchDirection,
    RollDirection,
    YawDirection
} from './types'
import axios from 'axios'

class Api {
    stop(): Promise<void> {
        return axios.post('/api/move/stop')
    }

    moveForward(direction: DirectionForward, distance: number, speed: number): Promise<void> {
        if (direction === DirectionForward.forward) {
            return axios.post(`/api/move/forward/${distance}/${speed}`)
        } else if (direction === DirectionForward.backward) {
            return axios.post(`/api/move/backward/${distance}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    moveHorizontal(direction: DirectionHorizontal, distance: number, speed: number): Promise<void> {
        if (direction === DirectionHorizontal.left) {
            return axios.post(`/api/move/left/${distance}/${speed}`)
        } else if (direction === DirectionHorizontal.right) {
            return axios.post(`/api/move/right/${distance}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    moveVertical(direction: DirectionVertical, distance: number, speed: number): Promise<void> {
        if (direction === DirectionVertical.up) {
            return axios.post(`/api/move/up/${distance}/${speed}`)
        } else if (direction === DirectionVertical.down) {
            return axios.post(`/api/move/down/${distance}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    yaw(direction: YawDirection, steps: number, speed: number) {
        if (direction === YawDirection.left) {
            return axios.post(`/api/actuators/step/yaw/left/${steps}/${speed}`)
        } else if (direction === YawDirection.right) {
            return axios.post(`/api/actuators/step/yaw/right/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    lowerVert(direction: PitchDirection, steps: number, speed: number) {
        if (direction === PitchDirection.up) {
            return axios.post(`/api/actuators/step/lv/up/${steps}/${speed}`)
        } else if (direction === PitchDirection.down) {
            return axios.post(`/api/actuators/step/lv/down/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    upperVert(direction: PitchDirection, steps: number, speed: number) {
        if (direction === PitchDirection.up) {
            return axios.post(`/api/actuators/step/uv/up/${steps}/${speed}`)
        } else if (direction === PitchDirection.down) {
            return axios.post(`/api/actuators/step/uv/down/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    roll(direction: RollDirection, steps: number, speed: number) {
        if (direction === RollDirection.ccw) {
            return axios.post(`/api/actuators/step/roll/ccw/${steps}/${speed}`)
        } else if (direction === RollDirection.cw) {
            return axios.post(`/api/actuators/step/roll/cw/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    pitch(direction: PitchDirection, steps: number, speed: number) {
        if (direction === PitchDirection.up) {
            return axios.post(`/api/actuators/step/pitch/up/${steps}/${speed}`)
        } else if (direction === PitchDirection.down) {
            return axios.post(`/api/actuators/step/pitch/down/${steps}/${speed}`)
        } else {
            throw 'invalid direction'
        }
    }

    goHome(speed: number): Promise<void> {
        return axios.post(`/api/home/${speed}`)
    }

    setHome(): Promise<void> {
        return axios.post('/api/actuators/set-home')
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
