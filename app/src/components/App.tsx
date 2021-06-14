import React, {useState} from 'react'
import {Button, InputNumber} from 'antd'
import {useDispatch, useSelector} from 'react-redux'
import {goHome, moveYaw, setHome} from '../actions'
import {State} from '../reducer'
import {GO_HOME_KEY, SET_HOME_KEY,} from '../key-bindings'
import NumStepsSelector from './NumStepsSelector'
import StepBaseButtons from './StepBaseButtons'
import StepHeadButtons from './StepHeadButtons'
import SteppingSpeedSlider from './SteppingSpeedSlider'
import {HomeOutlined} from '@ant-design/icons'
import RotationTable from './RotationTable'
import RotationDials from './RotationDials'
import Position from './PositionDisplay'
import PositionDisplay from './PositionDisplay'
import MoveToPanel from './MoveToPanel'

const appStyle: React.CSSProperties = {
    display: 'flex',
    flexDirection: 'column',
    margin: '1em',
}

const App: React.FunctionComponent = () => {
    const dispatch = useDispatch()
    const [yawAngle, setYawAngle] = useState(0)

    return <div style={appStyle}>
        <NumStepsSelector/>
        <div style={{display: 'flex', flexDirection: 'row', alignItems: 'center'}}>
            <SteppingSpeedSlider />
            <StepBaseButtons />
            <StepHeadButtons />
            <PositionDisplay />
        </div>
        <div>
            <Button onClick={() => dispatch(setHome())}><HomeOutlined /> set home ({SET_HOME_KEY})</Button>
            <Button onClick={() => dispatch(goHome())}><HomeOutlined /> home ({GO_HOME_KEY})</Button>
        </div>
        <MoveToPanel/>
        <RotationTable/>

        <div>
            yaw: <InputNumber value={yawAngle} onChange={setYawAngle}/> <Button onClick={() => dispatch(moveYaw(yawAngle))}>set to {yawAngle}</Button>
        </div>
    </div>
}
App.displayName = 'App'
export default App