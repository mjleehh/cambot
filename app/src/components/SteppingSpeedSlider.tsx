import {Button, Slider} from 'antd'
import {decreaseSpeed, increaseSpeed, setSpeed} from '../actions'
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
    const speed = useSelector<State, number>(({speed}) => speed)

    return <div style={style}>
        <Button icon={<PlusOutlined />} onClick={() => dispatch(increaseSpeed())} />
        <Slider style={sliderStyle} vertical min={1} max={1000} value={speed} onChange={(value: number) => dispatch(setSpeed(value))}/>
        <Button icon={<MinusOutlined />} onClick={() => dispatch(decreaseSpeed())} />
    </div>
}
SteppingSpeedSlider.displayName = 'SteppingSpeedSlider'
export default SteppingSpeedSlider