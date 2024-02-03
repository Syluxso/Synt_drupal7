<?php

class NyTechAPIDeal extends eClass {

  public $stage;
  public $stage_fa;
  public $state;

  function __construct($entity) {
    parent::__construct($entity, 'deal', 'standard');
    $this->set_stage($entity);
    $this->set_state($entity);
    $this->set_urls($entity);
    }

  private function  set_stage($entity) {
    $this->stage = $entity->stage;
  }

  private function  set_state($entity) {
    $this->state = $entity->state;
  }

  private function set_urls($entity) {
    $root = root_url();
    $this->urls = [
      'view' => $root . 'deals/' . $this->entity_id . '/view',
      'archive' => $root . 'deals/' . $this->entity_id . '/archive',
    ];
  }
}
