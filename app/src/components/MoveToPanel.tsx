import React, {useState} from 'react'
import {useDispatch, useSelector} from 'react-redux'
import {State} from '../reducer'
import {Position} from '../types'
import {Button, Input} from 'antd'
import {moveTo} from '../actions'

const style: React.CSSProperties = {
    margin: '1em',
}

const MoveToPanel: React.FunctionComponent = () => {
    const dispatch = useDispatch()
    const {x, y, z} = useSelector<State, Position>(({armState: {position}}) => position)
    const [newPosition, setNewPosition] = useState({x, y, z})

    console.log(newPosition)
    return <div style={style}>
        <Input type='number' value={newPosition.x} onChange={e => setNewPosition({...newPosition, x: parseInt(e.target.value)})}/>
        <Input type='number' value={newPosition.y} onChange={e => setNewPosition({...newPosition, y: parseInt(e.target.value)})}/>
        <Input type='number' value={newPosition.z} onChange={e => setNewPosition({...newPosition, z: parseInt(e.target.value)})}/>
        <Button type='primary' onClick={() => dispatch(moveTo(newPosition.x, newPosition.y, newPosition.z, 0.05))}>go to</Button>
        <Button onClick={() => setNewPosition({x, y, z})}>set to current</Button>
    </div>
}
MoveToPanel.displayName = 'MoveToPanel'
export default MoveToPanel