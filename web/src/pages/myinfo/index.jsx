import React, { useContext, useMemo } from "react";
import { Button, Col, Form, Input, Row, App } from "antd";
import intl from "@/component/intl";
import authHOC from "../common/hoc/authHOC";
import RootContext from "../rootContext";
import styles from "./index.module.less";

const MyInfo = () => {
  const { message } = App.useApp();
  const { userInfo } = useContext(RootContext);
  const [form] = Form.useForm();

  const initialValues = useMemo(() => {
    return {
      firstName: "",
      lastName: "",
      email: "",
      phone: "",
      bio: "",
    };
  }, [userInfo]);

  const onFinish = async (values) => {
    console.log("myinfo.save", values);
    message.success(intl.get("myinfo_save_success"));
  };

  return (
    <div className={styles.page}>
      <div className={styles.container}>
        <div className={styles.contentWrap}>
          <div className={styles.topBar}>
            <div className={styles.headerInfo}>
              <div className={styles.headerTitle}>{intl.get("myinfo_page_title")}</div>
              <div className={styles.headerDesc}>{intl.get("myinfo_page_subtitle")}</div>
            </div>
          </div>

          <div className={styles.divider} />

          <div className={styles.formWrap}>
            <Form
              form={form}
              layout="vertical"
              initialValues={initialValues}
              onFinish={onFinish}
              requiredMark={false}
            >
              <Row gutter={[16, 18]}>
                <Col xs={24} sm={12}>
                  <Form.Item
                    label={intl.get("myinfo_first_name")}
                    name="firstName"
                    rules={[{ required: true, message: intl.get("myinfo_first_name_required") }]}
                  >
                    <Input placeholder={intl.get("myinfo_placeholder_input")} />
                  </Form.Item>
                </Col>
                <Col xs={24} sm={12}>
                  <Form.Item
                    label={intl.get("myinfo_last_name")}
                    name="lastName"
                    rules={[{ required: true, message: intl.get("myinfo_last_name_required") }]}
                  >
                    <Input placeholder={intl.get("myinfo_placeholder_input")} />
                  </Form.Item>
                </Col>
              </Row>

              <Form.Item
                label={intl.get("myinfo_email")}
                name="email"
                rules={[
                  { required: true, message: intl.get("myinfo_email_required") },
                  { type: "email", message: intl.get("myinfo_email_invalid") },
                ]}
              >
                <Input placeholder={intl.get("myinfo_placeholder_email")} />
              </Form.Item>

              <Form.Item label={intl.get("myinfo_phone")} name="phone">
                <Input placeholder={intl.get("myinfo_placeholder_phone")} />
              </Form.Item>

              <Form.Item label={intl.get("myinfo_bio")} name="bio">
                <Input.TextArea
                  rows={4}
                  placeholder={intl.get("myinfo_placeholder_bio")}
                />
              </Form.Item>

              <Form.Item className={styles.submitWrap}>
                <Button
                  type="primary"
                  htmlType="submit"
                  className={styles.submitBtn}
                >
                  {intl.get("myinfo_save_button")}
                </Button>
              </Form.Item>
            </Form>
          </div>
        </div>
      </div>
    </div>
  );
};

export default authHOC(true)(MyInfo);
