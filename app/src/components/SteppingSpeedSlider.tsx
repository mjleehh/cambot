import {Button, Slider} from 'antd'
import {decreaseRotationSpeed, increaseRotationSpeed, setRotationSpeed} from '../actions'
import React from 'react'
import {useDispatch, useSelector} from 'react-redux'
import {State} from '../reducer'
import {MinusOutlined, PlusOutlined} from '@ant-design/icons'

const style: React.CSSProperties = {
    height: '100%',
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center',
    margin: '1em',
}

const sliderStyle: React.CSSProperties = {
    height: '22em'
}

const SteppingSpeedSlider: React.FunctionComponent = () => {
    const dispatch = useDispatch()
    const speed = useSelector<State, number>(({rotationSpeed}) => rotationSpeed)

    return <div style={style}>
        <Button icon={<PlusOutlined />} onClick={() => dispatch(increaseRotationSpeed())} />
        <Slider style={sliderStyle} vertical min={1} max={1000} value={speed} onChange={(value: number) => dispatch(setRotationSpeed(value))}/>
        <Button icon={<MinusOutlined />} onClick={() => dispatch(decreaseRotationSpeed())} />
    </div>
}
SteppingSpeedSlider.displayName = 'SteppingSpeedSlider'
export default SteppingSpeedSlider