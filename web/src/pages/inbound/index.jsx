import React, { useContext, useState } from 'react'
import { useNavigate } from 'react-router-dom'
import { motion } from 'framer-motion'
import { Form, Input, Button, App } from 'antd'
import intl from '../../component/intl'
import GlassCard from '../../pages/home/components/GlassCard/GlassCard'
import EmptyState from '@/component/emptyState'
import styles from './Inbound.module.less'
import authHOC from '../common/hoc/authHOC'
import RootContext from '../rootContext'
import { Status } from '../../constants/auth'
import { storageApply } from '../../api/entityCard'
import { NFT_ENTRY } from '../routes'

const Inbound = () => {
  const { message } = App.useApp()
  const { authStatus } = useContext(RootContext)
  const navigate = useNavigate()
  const [email, setEmail] = useState('')
  const [isSubmitting, setIsSubmitting] = useState(false)
  const [form] = Form.useForm()

  if (authStatus !== Status.LOGIN) {
    return <EmptyState />
  }

  const handleSubmit = async (values) => {
    if (isSubmitting) return
    setIsSubmitting(true)
    try {
      await storageApply({ email: values?.email })
      message.success(intl.get('inbound_apply_success'))
      setEmail('')
      form.resetFields()
    } catch (e) {
      message.error(e?.errorMsg || e?.message || intl.get('inbound_action_failed_retry'))
    } finally {
      setIsSubmitting(false)
    }
  }

  const handleViewHistory = () => {
    navigate(`${NFT_ENTRY}?tab=history&historyTab=entity`)
  }

  const containerVariants = {
    hidden: { opacity: 0 },
    visible: {
      opacity: 1,
      transition: {
        staggerChildren: 0.1
      }
    }
  }

  const itemVariants = {
    hidden: { opacity: 0, y: 20 },
    visible: {
      opacity: 1,
      y: 0,
      transition: {
        duration: 0.6,
        ease: [0.4, 0, 0.2, 1]
      }
    }
  }

  const processSteps = [
    { id: 1, text: intl.get('inbound_process_1') },
    { id: 2, text: intl.get('inbound_process_2') },
    { id: 3, text: intl.get('inbound_process_3') },
    { id: 4, text: intl.get('inbound_process_4') },
    { id: 5, text: intl.get('inbound_process_5') },
    { id: 6, text: intl.get('inbound_process_6') }
  ]

  return (
    <div className={styles.container}>
      <motion.div
        className={styles.content}
        variants={containerVariants}
        initial="hidden"
        animate="visible"
      >
        <motion.div variants={itemVariants} className={styles.header}>
          <div className={styles.headerContent}>
            <div>
              <h1 className={styles.title}>{intl.get('inbound_title')}</h1>
              <p className={styles.description}>{intl.get('inbound_description')}</p>
            </div>
            <Button
              type="link"
              onClick={handleViewHistory}
            >
              {intl.get('view_inbound_history')}
            </Button>
          </div>
        </motion.div>

        <motion.div variants={itemVariants} className={styles.processSection}>
          <h2 className={styles.processTitle}>{intl.get('inbound_process_title')}</h2>
          <div className={styles.processSteps}>
            {processSteps.map((step, index) => (
              <div key={step.id} className={styles.processStep}>
                <div className={styles.stepNumber}>{step.id}</div>
                <div className={styles.stepText}>{step.text}</div>
                {index < processSteps.length - 1 && <div className={styles.stepConnector} />}
              </div>
            ))}
          </div>
        </motion.div>

        <motion.div variants={itemVariants} className={styles.formSection}>
          <GlassCard variant="default" className={styles.formCard}>
            <Form
              form={form}
              onFinish={handleSubmit}
              className={styles.form}
              layout="vertical"
            >
              <Form.Item
                name="email"
                rules={[
                  { required: true, message: intl.get('inbound_email_required') },
                  { type: 'email', message: intl.get('inbound_email_invalid') }
                ]}
                className={styles.formGroup}
              >
                <Input
                  type="email"
                  value={email}
                  onChange={(e) => setEmail(e.target.value)}
                  placeholder={intl.get('inbound_email_placeholder')}
                  className={styles.input}
                />
              </Form.Item>
              <Form.Item>
                <Button
                  type="primary"
                  size='large'
                  htmlType="submit"
                  loading={isSubmitting}
                  block
                  className={styles.submitButton}
                >
                  {intl.get('inbound_submit')}
                </Button>
              </Form.Item>
            </Form>
          </GlassCard>
        </motion.div>

        <motion.div variants={itemVariants} className={styles.noticeSection}>
          <GlassCard variant="default" className={styles.noticeCard}>
            <div className={styles.noticeText}>{intl.get('inbound_notice')}</div>
          </GlassCard>
        </motion.div>
      </motion.div>
    </div>
  )
}

export default authHOC(true)(Inbound)
