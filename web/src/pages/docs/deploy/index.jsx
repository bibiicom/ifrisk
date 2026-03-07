import React, { useState, useEffect } from 'react'

import { useParams } from 'react-router-dom'

import showdown from 'showdown'

import Layout from '../Layout'

import { getDocs } from '../../../api/docs'

import authHOC from '../../common/hoc/authHOC'

export default authHOC(true)(() => {
  const { key } = useParams()

  const [loading, setLoading] = useState(true)

  const [views, setViews] = useState('')

  useEffect(async () => {
    try {
      const result = await getDocs(key)

      const converter = new showdown.Converter()
      const html = converter.makeHtml(result.result)
      setViews(html)
      setLoading(false)
    } catch (e) {
      setLoading(false)
    }
  }, [key])

  return (
    <Layout
      loading={loading}>
      {
        views &&
        <p className="markdown-body" dangerouslySetInnerHTML={{ __html: views }}/>
      }
    </Layout>
  )
})
