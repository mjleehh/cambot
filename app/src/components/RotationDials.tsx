import React, {useEffect, useRef} from 'react'
import {useSelector} from 'react-redux'
import {State} from '../reducer'
import {Rotations} from '../types'
import _ from 'lodash'
import {Table} from 'antd'

const RotationDials: React.FunctionComponent = (props) => {
    const canvasRef = useRef(null)
    const rotations = useSelector<State, Rotations>(({rotations}) => rotations)

    useEffect(() => {
        const canvas = canvasRef.current
        const context = canvas.getContext('2d')
        //Our first draw
        context.fillStyle = '#000000'
        context.fillRect(0, 0, context.canvas.width, context.canvas.height)

    }, [])

    return <canvas ref={canvasRef} {...props}/>
}
RotationDials.displayName = 'RotationDials'
export default RotationDials